const knx = require('knx')
const dptlib = require('knx/src/dptlib')
const oOS = require('os')

module.exports = (RED) => {

    function crestronConfigNode(config) {
        RED.nodes.createNode(this, config)
        var node = this
        node.host = config.host
        node.port = config.port

        node.setAllClientsStatus = (_status, _color, _text) => {
            function nextStatus(oClient) {
                oClient.setNodeStatus({ fill: _color, shape: "dot", text: _status + " " + _text })
            }
            node.nodeClients.map(nextStatus);
        }

        node.Disconnect = () => {
            node.setAllClientsStatus("Waiting", "grey", "")
            // Remove listener
            try {
                node.crestronConnection.removeListener("event");
            } catch (error) {

            }
            try {
                node.crestronConnection.off("event");
            } catch (error) {

            }
            node.linkStatus = "disconnected"; // 29/08/2019 signal disconnection
            try {
                node.crestronConnection.Disconnect();
            } catch (error) {
            }

            node.crestronConnection = null;
        }

        node.addClient = (_Node) => {
            // Check if node already exists
            if (node.nodeClients.filter(x => x.id === _Node.id).length === 0) {
                // Check if the node has a valid topic and dpt

                if (typeof _Node.topic == "undefined" || typeof _Node.dpt == "undefined") {
                    _Node.setNodeStatus({ fill: "red", shape: "dot", text: "" })
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
            if (_Node.initialread) {
                node.readValue(_Node.topic);
            }
        }


        node.removeClient = (_Node) => {
            // Remove the client node from the clients array
            //RED.log.info( "BEFORE Node " + _Node.id + " has been unsubscribed from receiving KNX messages. " + node.nodeClients.length);
            try {
                node.nodeClients = node.nodeClients.filter(x => x.id !== _Node.id)
            } catch (error) { }
            //RED.log.info("AFTER Node " + _Node.id + " has been unsubscribed from receiving KNX messages. " + node.nodeClients.length);

            // If no clien nodes, disconnect from bus.
            if (node.nodeClients.length === 0) {
                node.linkStatus = "disconnected";
                node.Disconnect();
            }
        }


        node.readInitialValues = () => {
            if (node.linkStatus !== "connected") return; // 29/08/2019 If not connected, exit
            if (node.crestronConnection) {
                var readHistory = [];
                let delay = 0;
                node.nodeClients
                    .filter(oClient => oClient.initialread)
                    .forEach(oClient => {
                        if (oClient.listenallga == true) {
                            delay = delay + 200
                            for (let index = 0; index < node.csv.length; index++) {
                                const element = node.csv[index];
                                if (readHistory.includes(element.ga)) return
                                setTimeout(() => node.readValue(element.ga), delay)
                                readHistory.push(element.ga)
                            }
                        } else {
                            if (readHistory.includes(oClient.topic)) return
                            setTimeout(() => node.readValue(oClient.topic), delay)
                            delay = delay + 200
                            readHistory.push(oClient.topic)
                        }

                    })
            }
        }


        node.readValue = topic => {
            if (node.linkStatus !== "connected") return; // 29/08/2019 If not connected, exit
            if (node.crestronConnection) {
                try {
                    node.crestronConnection.read(topic)
                } catch (error) {
                    RED.log.error('knxUltimate: readValue: (' + topic + ') ' + error);
                }

            }
        }


        node.initcrestronConnection = () => {
            node.Disconnect();
            node.setAllClientsStatus("Waiting", "grey", "")

            var crestronConnectionProperties = {
                ipAddr: node.host,
                ipPort: node.port,
                handlers: {
                    connected: () => {
                        node.linkStatus = "connected";
                        node.setAllClientsStatus("Connected", "green", "Waiting for telegram.")
                        node.readInitialValues() // Perform initial read if applicable
                    },
                    error: function (connstatus) {

                        node.linkStatus = "disconnected";
                        if (connstatus == "E_KNX_CONNECTION") {
                            setTimeout(() => node.setAllClientsStatus(connstatus, "red", "Error on KNX BUS. Check KNX red/black connector and cable."), 2000)
                            RED.log.error("knxUltimate: Bind KNX Bus to interface error: " + connstatus);
                        } else {
                            setTimeout(() => node.setAllClientsStatus(connstatus, "red", "Error"), 2000)
                            RED.log.error("knxUltimate: crestronConnection error: " + connstatus);
                        }

                    }
                }
            };

            node.crestronConnection = new knx.Connection(crestronConnectionProperties);

            // Handle BUS events
            node.crestronConnection.on("event", function (evt, src, dest, rawValue) {
                // if (dest == "0/0/50") RED.log.error("RX FROM BUS : " + src + " " + dest + " " + evt + rawValue);
                // if (dest == "0/0/50") {
                //     node.nodeClients.filter(input => input.notifywrite == true).forEach(input => {
                //     RED.log.error("ID=" + input.id + " " + input.topic + " dest=" + dest + " notifywrite=" + input.notifywrite + " listenallga="+input.listenallga);
                //     });
                // }
                switch (evt) {
                    case "GroupValue_Write": {
                        node.nodeClients
                            .filter(input => input.notifywrite == true)
                            .forEach(input => {
                                if (input.listenallga == true) {
                                    // Get the GA from CVS
                                    let oGA;
                                    try {
                                        oGA = node.csv.filter(sga => sga.ga == dest)[0];
                                    } catch (error) { }

                                    // 25/10/2019 TRY TO AUTO DECODE
                                    // --------------------------------
                                    if (typeof oGA === "undefined") {
                                        // 25/10/2019 from v. 1.1.11, try to decode and output a datapoint.
                                        let msg = buildInputMessage(src, dest, evt, rawValue, tryToFigureOutDataPointFromRawValue(rawValue, dest), "")
                                        input.setNodeStatus({ fill: "green", shape: "dot", text: "Try to decode", payload: msg.payload, GA: msg.knx.destination, dpt: "", devicename: "" });
                                        input.send(msg)
                                        // --------------------------------

                                    } else {
                                        let msg = buildInputMessage(src, dest, evt, rawValue, oGA.dpt, oGA.devicename)
                                        input.setNodeStatus({ fill: "green", shape: "dot", text: "", payload: msg.payload, GA: msg.knx.destination, dpt: msg.knx.dpt, devicename: msg.devicename });
                                        input.send(msg)
                                    }
                                } else if (input.topic == dest) {

                                    let msg = buildInputMessage(src, dest, evt, rawValue, input.dpt, input.name ? input.name : "")
                                    // Check RBE INPUT from KNX Bus, to avoid send the payload to the flow, if it's equal to the current payload
                                    if (!checkRBEInputFromKNXBusAllowSend(input, msg.payload)) {
                                        input.setNodeStatus({ fill: "grey", shape: "ring", text: "rbe block (" + msg.payload + ") from KNX", payload: "", GA: "", dpt: "", devicename: "" })
                                        return;
                                    };
                                    input.currentPayload = msg.payload;// Set the current value for the RBE input
                                    input.setNodeStatus({ fill: "green", shape: "dot", text: "", payload: msg.payload, GA: input.topic, dpt: input.dpt, devicename: "" });
                                    //RED.log.error("RX FROM BUS : " + input.id +" " + src + " " + dest + " " + evt)
                                    input.send(msg)
                                }
                            })
                        break;
                    }
                    case "GroupValue_Response": {

                        node.nodeClients
                            .filter(input => input.notifyresponse == true)
                            .forEach(input => {
                                if (input.listenallga == true) {
                                    // Get the DPT
                                    let oGA;
                                    try {
                                        oGA = node.csv.filter(sga => sga.ga == dest)[0];
                                    } catch (error) { }

                                    // 25/10/2019 TRY TO AUTO DECODE
                                    // --------------------------------
                                    if (typeof oGA === "undefined") {
                                        let msg = buildInputMessage(src, dest, evt, rawValue, tryToFigureOutDataPointFromRawValue(rawValue, dest), "")
                                        input.setNodeStatus({ fill: "green", shape: "dot", text: "Try to decode", payload: msg.payload, GA: msg.knx.destination, dpt: "", devicename: "" });
                                        input.send(msg)
                                        // --------------------------------

                                    } else {
                                        let msg = buildInputMessage(src, dest, evt, rawValue, oGA.dpt, oGA.devicename)
                                        input.setNodeStatus({ fill: "blue", shape: "dot", text: "", payload: msg.payload, GA: msg.knx.destination, dpt: msg.knx.dpt, devicename: msg.devicename });
                                        input.send(msg)
                                    }
                                } else if (input.topic == dest) {
                                    let msg = buildInputMessage(src, dest, evt, rawValue, input.dpt, input.name ? input.name : "")
                                    // Check RBE INPUT from KNX Bus, to avoid send the payload to the flow, if it's equal to the current payload
                                    if (!checkRBEInputFromKNXBusAllowSend(input, msg.payload)) {
                                        input.setNodeStatus({ fill: "grey", shape: "ring", text: "rbe INPUT filter applied on " + msg.payload })
                                        return;
                                    };
                                    input.currentPayload = msg.payload; // Set the current value for the RBE input
                                    input.setNodeStatus({ fill: "blue", shape: "dot", text: "", payload: msg.payload, GA: input.topic, dpt: msg.knx.dpt, devicename: msg.devicename });
                                    input.send(msg)
                                }
                            })
                        break;
                    }
                    case "GroupValue_Read": {

                        node.nodeClients
                            .filter(input => input.notifyreadrequest == true)
                            .forEach(input => {
                                if (input.listenallga == true) {
                                    // Get the DPT
                                    let oGA;
                                    try {
                                        oGA = node.csv.filter(sga => sga.ga == dest)[0];
                                    } catch (error) { }

                                    // 25/10/2019 TRY TO AUTO DECODE
                                    // --------------------------------
                                    if (typeof oGA === "undefined") {
                                        // 25/10/2019 from v. 1.1.11, try to decode and output a datapoint.
                                        let msg = buildInputMessage(src, dest, evt, null, tryToFigureOutDataPointFromRawValue(rawValue, dest), "")
                                        input.setNodeStatus({ fill: "green", shape: "dot", text: "Try to decode", payload: msg.payload, GA: msg.knx.destination, dpt: "", devicename: "" });
                                        input.send(msg)
                                        // --------------------------------

                                    } else {
                                        let msg = buildInputMessage(src, dest, evt, null, oGA.dpt, oGA.devicename)
                                        input.setNodeStatus({ fill: "grey", shape: "dot", text: "Read", payload: msg.payload, GA: msg.knx.destination, dpt: msg.knx.dpt, devicename: msg.devicename });
                                        input.send(msg)
                                    }
                                } else if (input.topic == dest) {
                                    let msg = buildInputMessage(src, dest, evt, null, input.dpt, input.name ? input.name : "")
                                    // 24/09/2019 Autorespond to BUS
                                    if (input.notifyreadrequestalsorespondtobus === true) {
                                        if (typeof input.currentPayload === "undefined" || input.currentPayload === "") {
                                            setTimeout(() => {
                                                node.crestronConnection.respond(dest, input.notifyreadrequestalsorespondtobusdefaultvalueifnotinitialized, input.dpt);
                                                input.setNodeStatus({ fill: "blue", shape: "ring", text: "Read & Autorespond with default", payload: input.notifyreadrequestalsorespondtobusdefaultvalueifnotinitialized, GA: input.topic, dpt: msg.knx.dpt, devicename: "" });
                                            }, 200);
                                        } else {
                                            setTimeout(() => {
                                                node.crestronConnection.respond(dest, input.currentPayload, input.dpt);
                                                input.setNodeStatus({ fill: "blue", shape: "ring", text: "Read & Autorespond", payload: input.currentPayload, GA: input.topic, dpt: msg.knx.dpt, devicename: "" });
                                            }, 200);
                                        }
                                    } else {
                                        input.setNodeStatus({ fill: "grey", shape: "dot", text: "Read", payload: msg.payload, GA: input.topic, dpt: msg.knx.dpt, devicename: "" });
                                    }
                                    input.send(msg)

                                }
                            })
                        break;
                    }
                    default: return
                }
            })
        }

        function buildInputMessage(src, dest, evt, value, inputDpt, _devicename) {
            // Resolve DPT and convert value if available
            //if (dest=="0/0/50") RED.log.error("Buildinputmessage src=" + src + " dest" + dest + " value=" + value + " inputDpt=" + inputDpt + " _devicename="+_devicename);
            var dpt = dptlib.resolve(inputDpt)
            var jsValue = null
            if (dpt && value) {
                var jsValue = dptlib.fromBuffer(value, dpt)
            }

            // Build final input message object
            return {
                topic: dest
                , payload: jsValue
                , knx:
                {
                    event: evt
                    , dpt: inputDpt
                    //, dptDetails: dpt
                    , source: src
                    , destination: dest
                    , rawValue: value
                }
                , devicename: (typeof _devicename !== 'undefined') ? _devicename : ""
            }
        }


        node.on("close", function () {
            node.Disconnect();
        })
    }

    RED.nodes.registerType("crestron-node-config", crestronConfigNode);
}