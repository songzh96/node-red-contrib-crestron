const net = require('net')
const async = require('async');
require('events').EventEmitter.setMaxListeners = 15;

module.exports = (RED) => {

    function crestronConfigNode(config) {
        RED.nodes.createNode(this, config)
        var node = this
        node.host = config.host
        node.port = config.port
        node.nodeClients = [] // Stores the registered clients

        // set node status
        node.setAllClientsStatus = (_status, _color, _text) => {
            function nextStatus(oClient) {
                oClient.setNodeStatus({ fill: _color, shape: "dot", text: _status + " " + _text })
            }
            node.nodeClients.map(nextStatus);
        }

        // node disconnect event
        node.Disconnect = () => {
            node.setAllClientsStatus("Waiting", "grey", "")

            // Remove listener
            try {
                node.crestronConnection.removeListener("data");
            } catch (error) {

            }
            try {
                node.crestronConnection.removeListener("close");
            } catch (error) {

            }
            try {
                node.crestronConnection.removeListener("connect");
            } catch (error) {

            }
            try {
                node.crestronConnection.removeListener("error");
            } catch (error) {

            }
            node.linkStatus = "disconnected";
            try {
                node.crestronConnection.destroy();
            } catch (error) {

            }

            node.crestronConnection = null;
        }

        node.addClient = (_Node) => {
            // Check if node already exists
            if (node.nodeClients.filter(x => x.id === _Node.id).length === 0) {
                // Check if the node has a valid topic and dpt

                if (typeof _Node.cid == "undefined") {
                    _Node.setNodeStatus({ fill: "red", shape: "dot", text: "ID is Null" })
                    return;
                } else {


                }

                // Add _Node to the clients array
                node.nodeClients.push(_Node)
            }
            // At first node client connection, this node connects to the bus
            if (node.nodeClients.length === 1) {
                // 14/08/2018 Initialize the connection
                node.initcrestronConnection();
            }

        }


        node.removeClient = (_Node) => {
            // Remove the client node from the clients array
            //RED.log.info( "BEFORE Node " + _Node.id + " has been unsubscribed from receiving KNX messages. " + node.nodeClients.length);
            try {
                node.nodeClients = node.nodeClients.filter(x => x.id !== _Node.id)
            } catch (error) {
                // console.log(error);
            }
            //RED.log.info("AFTER Node " + _Node.id + " has been unsubscribed from receiving KNX messages. " + node.nodeClients.length);

            // If no clien nodes, disconnect from bus.
            if (node.nodeClients.length === 0) {
                
                node.linkStatus = "disconnected";
                node.Disconnect();
            }
        }


        // node.readInitialValues = () => {
        //     if (node.linkStatus !== "connected") return; // 29/08/2019 If not connected, exit
        //     if (node.crestronConnection) {
        //         var readHistory = [];
        //         let delay = 0;
        //         node.nodeClients
        //             .filter(oClient => oClient.initialread)
        //             .forEach(oClient => {

        //                 if (readHistory.includes(oClient.topic)) return
        //                 setTimeout(() => node.readValue(oClient.topic), delay)
        //                 delay = delay + 200
        //                 readHistory.push(oClient.topic)
        //             })
        //     }
        // }

        node.initcrestronConnection = () => {
            node.Disconnect();
            node.setAllClientsStatus("Waiting", "grey", "")

            //create TCP conn
            node.crestronConnection = new net.Socket();

            // Connection event
            node.crestronConnection.connect(node.port, node.host, function () {
                RED.log.info("Connected to Crestron Machine");
            });

            // Connection success event
            node.crestronConnection.on("connect", function () {
                // update node status
                node.linkStatus = "connected";
                RED.log.info('Connected to Crestron Processor');
                node.setAllClientsStatus("Connected", "green", "Waiting for message.")
                // node.readInitialValues() // Perform initial read if applicable
            });

            // Listen for messages from the server
            node.crestronConnection.on("data", function (data) {

                var dataArray = data.toString().split("*"); // Commands terminated with *

                async.each(dataArray, function (response, callback) {
                    // parse msg
                    var responseArray = response.toString().split(":");
                    // responseArray[0] = (node.ctype ie Digital) : responseArray[1] = (node.cid) : responseArray[2] = (command ie event) : responseArray[3] = (value)
                    let type = responseArray[0];
                    let id = responseArray[1];
                    let evt = responseArray[2];
                    let value = responseArray[3];
                    switch (type) {
                        case "Digital":
                            {
                                node.nodeClients
                                    .forEach(input => {
                                        if (id === input.cid && type === input.ctype) {
                                            
                                            let msg = buildInputMessage(input.ctype, input.cid, "Event", Boolean(value), input.name)
                                            
                                            input.setNodeStatus({ fill: "green", shape: "dot", text: "Digital msg is coming" });
                                            input.send(msg);
                                        }
                                    });
                                break;
                            }
                        case "Analog":
                            {
                                node.nodeClients
                                    .forEach(input => {
                                        if (id === input.cid && type === input.ctype) {
                                            value = Number(value);
                                            if (value >= 0 && value <= 65535)
                                            {
                                                let msg = buildInputMessage(input.ctype, input.cid, "Event", value, input.name)
                                                input.setNodeStatus({ fill: "green", shape: "dot", text: "Analog msg is coming" });
                                                input.send(msg);
                                            }
                                            else
                                            {
                                                RED.log.warn("Value is out of range");
                                            }
                                            
                                        }
                                        
                                    });
                                break;
                            }
                        case "Serial":
                            {
                                
                                node.nodeClients
                                    .forEach(input => {
                                        if (id === input.cid && type === input.ctype) {
                                            let msg = buildInputMessage(input.ctype, input.cid, "Event", value, input.name)
                                            input.setNodeStatus({ fill: "green", shape: "dot", text: "Serial msg is coming" });
                                            input.send(msg);
                                        }
                                    });
                                break;
                            }
                        default: return;
                    }
                    callback();

                });
            });

            // When the conn is close
            node.crestronConnection.on("close", function () {
                RED.log.info("Crestron Connection closed");
                // update node status
                node.linkStatus = "disconnected";
                node.setAllClientsStatus("Disconnected", "red", "Connection Closed");

                node.linkStatus = "reconnect";
                setTimeout(() => node.setAllClientsStatus("Reconnect", "grey", "try reconnect crestron professor"), 3000);
                
                // try reconnect（5s）
                if(node.crestronConnection)
                {
                    try {
                        setTimeout(() => node.crestronConnection.connect(node.port, node.host, function () {
                            RED.log.info('Re-Connected to Crestron Machine');
                        }.bind(this)), 5000);
    
                    } catch (err) {
                        RED.log.error(err);
                    }
                }
                else{

                }
            });

            node.crestronConnection.on("error", function (err) {
                RED.log.error(err);
                node.crestronConnection.destroy();
            });

        }



        function buildInputMessage(type, id, evt, value, _devicename) {

            var jsValue = null;
            jsValue = value;
            // Build final input message object
            return {
                payload: jsValue,
                crestron:
                {
                    type: type,
                    id: id,
                    evt: evt,
                    rawValue: value
                }
                , devicename: (typeof _devicename !== 'undefined') ? _devicename : ""
            }
        }


        node.on("close", function () {
            
            node.Disconnect();
        });
    }

    RED.nodes.registerType("crestron-node-config", crestronConfigNode);
}