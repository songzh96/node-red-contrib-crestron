const knx = require('knx')
const dptlib = require('knx/src/dptlib')
const oOS = require('os')
'use strict';

const async = require('async');
const request = require("request");
const net = require('net');
const events = require('events');

// TCP connection to Crestron Module
var cresKitSocket = new net.Socket();
var eventEmitter = new events.EventEmitter();


var eventCheckData = [];
function fromEventCheck(what) {
    var found = eventCheckData.indexOf(what);
    var originalFound = found;
    while (found !== -1) { // Remove all references
        eventCheckData.splice(found, 1);
        found = eventCheckData.indexOf(what);
    }
    if (originalFound==-1) { // No match
        return false;
    } else {
        return true;
    }
}

var openGetStatus = []; // Sometimes a getStatus does not come back. We need to re-try for the app to be responsive.
function closeGetStatus(what) {
    var found = openGetStatus.indexOf(what);
    openGetStatus.splice(found, 1);

    console.log(openGetStatus);
}

// Resend unclosed GetStatus
function retryGetStatus() {
    async.each(openGetStatus, function (writeString, callback) {
        try {
            cresKitSocket.write(writeString);
            console.log("RETRY: " + writeString);
        } catch (err) {
            console.log(err);
        }
        callback();
    }.bind(this), function (err) {
        //console.log("retryGetStatus complete");
    });
}
setInterval(function() { retryGetStatus(); }, 2000);

cresKitSocket.connect(this.config["port"], this.config["host"], function() {
    this.log('Connected to Crestron Machine');
    // ERROR CONNECITON
}.bind(this));

cresKitSocket.on('close', function() {
    this.log('Connection closed');
    // Handle error properly
    // Reconnect
    try {
        cresKitSocket.connect(this.config["port"], this.config["host"], function() {
            this.log('Re-Connected to Crestron Machine');
        }.bind(this));
    } catch (err) {
        this.log(err);
    }


}.bind(this));

// All Crestron replies goes via this connection
cresKitSocket.on('data', function(data) {
    //this.log("Raw Crestron Data : " + data);

    // Data from Creston Module. This listener parses the information and updates Homebridge
    // get* - replies from get* requests
    // event* - sent upon any changes on Crestron side (including in response to set* commands)
    var dataArray = data.toString().split("*"); // Commands terminated with *
    async.each(dataArray, function(response, callback) {
        var responseArray = response.toString().split(":");
        // responseArray[0] = (config.type ie lightbulbs) : responseArray[1] = (id) : responseArray[2] = (command ie getPowerState) : responseArray[3] = (value)

        if (responseArray[0]!="") {
            eventEmitter.emit(responseArray[0] + ":" + responseArray[1] + ":" + responseArray[2], parseInt(responseArray[3])); // convert string to value
            //this.log("EMIT: " + responseArray[0] + ":" + responseArray[1] + ":" + responseArray[2] + " = " + responseArray[3]);
        }

        callback();

    }.bind(this), function(err) {
        //console.log("SockedRx Processed");
    });

}.bind(this));

// Accessories Configuration
async.each(this.config.accessories, function(accessory, asynCallback) {

    var accessory = new CresKitAccessory( this.log, this.config, accessory);
    foundAccessories.push(accessory);

    return asynCallback();  //let async know we are done
}.bind(this), function(err) {

    if(err) {
        this.log(err);
    } else {
        this.log("Success CresKit Config");
        callback(foundAccessories);
    }
}.bind(this));

}


module.exports = (RED) => {


    function knxUltimateConfigNode(config) {
        RED.nodes.createNode(this, config)
        var node = this
        node.host = config.host
        node.port = config.port
        
        
        node.setAllClientsStatus = (_status, _color, _text) => {
            function nextStatus(oClient) {
                oClient.setNodeStatus({ fill: _color, shape: "dot", text: _status + " " + _text ,payload: "", GA: oClient.topic, dpt:"", devicename:""})
            }
            node.nodeClients.map(nextStatus);
        }

        node.Disconnect = () => {
            node.setAllClientsStatus("Waiting", "grey", "")
            // Remove listener
            try {
                node.tcpConnection.removeListener("event");    
            } catch (error) {
                
            }
            try {
                node.tcpConnection.off("event");
            } catch (error) {
                
            }
            node.linkStatus = "disconnected"; // 2019/11/20 signal disconnection
            try {
                node.tcpConnection.Disconnect();
            } catch (error) {
            }
            
            node.tcpConnection = null;
        }

        node.addClient = (_Node) => {
            // Check if node already exists
            if (node.nodeClients.filter(x => x.id === _Node.id).length === 0) {
                // Check if the node has a valid topic and dpt
                if (_Node.listenallga==false) {
                    if (typeof _Node.topic == "undefined" || typeof _Node.dpt == "undefined") {
                        _Node.setNodeStatus({ fill: "red", shape: "dot", text: "Empty group address (topic) or datapoint.",payload: "", GA: "", dpt:"", devicename:"" })
                        return;
                    } else {
            
                        // Topic must be in formar x/x/x
                        if (_Node.topic.split("\/").length < 3) {
                            _Node.setNodeStatus({ fill: "red", shape: "dot", text: "Wrong group address (topic: " + _Node.topic + ") format.",payload: "", GA: "", dpt:"", devicename:"" })
                            return;
                        }
                    }
                }
                // Add _Node to the clients array
                node.nodeClients.push(_Node)
            }
            // At first node client connection, this node connects to the bus
            if (node.nodeClients.length === 1) {
                // 2019/11/20 Initialize the connection
                node.initTCPConnection();
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
            } catch (error) {}
            //RED.log.info("AFTER Node " + _Node.id + " has been unsubscribed from receiving KNX messages. " + node.nodeClients.length);

              // If no clien nodes, disconnect from bus.
            if (node.nodeClients.length === 0) {
                node.linkStatus = "disconnected";
                node.Disconnect();
            }
        }
      
        
        node.readInitialValues = () => {
            if (node.linkStatus !== "connected") return; // 29/08/2019 If not connected, exit
            if (node.tcpConnection) { 
                var readHistory = [];
                let delay = 0;
                node.nodeClients
                    .filter(oClient => oClient.initialread)
                    .forEach(oClient => {
                        if (oClient.listenallga==true) {
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
            if (node.tcpConnection) {
                try {
                    node.tcpConnection.read(topic)
                } catch (error) {
                    RED.log.error('knxUltimate: readValue: (' + topic + ') ' + error);
                }
                
            }
        }
        
            
        node.initTCPConnection = () => {
            node.Disconnect();
            node.setAllClientsStatus("Waiting", "grey", "")

            var tcpConnectionProperties = {
                ipAddr: node.host,
                ipPort: node.port,
                handlers: {
                    connected: () => {
                        node.linkStatus = "connected";
                        node.setAllClientsStatus("Connected", "green", "Waiting for telegram.")
                        node.readInitialValues() // Perform initial read if applicable
                    },
                    error: function (connstatus) {
                        // NO_ERROR: 0x00, // E_NO_ERROR - The connection was established succesfully
                        // E_HOST_PROTOCOL_TYPE: 0x01,
                        // E_VERSION_NOT_SUPPORTED: 0x02,
                        // E_SEQUENCE_NUMBER: 0x04,
                        // E_CONNSTATE_LOST: 0x15, // typo in eibd/libserver/eibnetserver.cpp:394, forgot 0x prefix ??? "uchar res = 21;"
                        // E_CONNECTION_ID: 0x21, // - The KNXnet/IP server device could not find an active data connection with the given ID
                        // E_CONNECTION_TYPE: 0x22, // - The requested connection type is not supported by the KNXnet/IP server device
                        // E_CONNECTION_OPTION: 0x23, // - The requested connection options is not supported by the KNXnet/IP server device
                        // E_NO_MORE_CONNECTIONS: 0x24, //  - The KNXnet/IP server could not accept the new data connection (Maximum reached)
                        // E_DATA_CONNECTION: 0x26,// - The KNXnet/IP server device detected an erro concerning the Dat connection with the given ID
                        // E_KNX_CONNECTION: 0x27,  // - The KNXnet/IP server device detected an error concerning the KNX Bus with the given ID
                        // E_TUNNELING_LAYER: 0x29,
                        node.linkStatus = "disconnected";
                        if (connstatus == "E_KNX_CONNECTION") {
                            setTimeout(() => node.setAllClientsStatus(connstatus, "red", "Error on KNX BUS. Check KNX red/black connector and cable."), 2000)
                            RED.log.error("knxUltimate: Bind KNX Bus to interface error: " + connstatus);
                        } else {
                            setTimeout(() => node.setAllClientsStatus(connstatus, "red", "Error"), 2000)
                            RED.log.error("knxUltimate: tcpConnection error: " + connstatus);
                        }
                        
                    }
                }
            };

            if (node.KNXEthInterface !== "Auto")
            {
                var sIfaceName = "";
                if (node.KNXEthInterface === "Manual") {
                    sIfaceName = node.KNXEthInterfaceManuallyInput;
                    RED.log.info("knxUltimate: Bind KNX Bus to interface : " + sIfaceName + " (Interface's name entered by hand)");
                } else {
                    sIfaceName = node.KNXEthInterface;
                    RED.log.info("knxUltimate: Bind KNX Bus to interface : " + sIfaceName + " (Interface's name selected from dropdown list)");
                }

                tcpConnectionProperties.interface = sIfaceName;            
            } else {
                RED.log.info("knxUltimate: Bind KNX Bus to interface (Auto)");
            }

            node.tcpConnection = new knx.Connection(tcpConnectionProperties);
         
            // Handle BUS events
            node.tcpConnection.on("event", function (evt, src, dest, rawValue) {
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
                                        oGA=node.csv.filter(sga => sga.ga == dest)[0];
                                    } catch (error) { }
                                    
                                    // 25/10/2019 TRY TO AUTO DECODE
                                    // --------------------------------
                                    if (typeof oGA === "undefined") {
                                        // 25/10/2019 from v. 1.1.11, try to decode and output a datapoint.
                                        let msg = buildInputMessage(src, dest, evt, rawValue, tryToFigureOutDataPointFromRawValue(rawValue,dest), "")
                                        input.setNodeStatus({ fill: "green", shape: "dot", text: "Try to decode",payload: msg.payload, GA: msg.knx.destination, dpt:"", devicename:"" });
                                        input.send(msg)    
                                      // --------------------------------
                                        
                                    } else {
                                        let msg = buildInputMessage(src, dest, evt, rawValue, oGA.dpt, oGA.devicename)
                                        input.setNodeStatus({ fill: "green", shape: "dot", text: "",payload: msg.payload, GA: msg.knx.destination, dpt:msg.knx.dpt, devicename:msg.devicename });
                                        input.send(msg)
                                    }
                                } else if (input.topic == dest) {
                                    
                                    let msg = buildInputMessage(src, dest, evt, rawValue, input.dpt, input.name ? input.name : "")
                                    // Check RBE INPUT from KNX Bus, to avoid send the payload to the flow, if it's equal to the current payload
                                    if (!checkRBEInputFromKNXBusAllowSend(input, msg.payload)) {
                                        input.setNodeStatus({fill: "grey", shape: "ring", text: "rbe block ("+msg.payload+") from KNX",payload: "", GA: "", dpt:"", devicename:""})
                                        return;
                                    };
                                    input.currentPayload = msg.payload;// Set the current value for the RBE input
                                    input.setNodeStatus({fill: "green", shape: "dot", text: "", payload: msg.payload, GA: input.topic, dpt:input.dpt, devicename:""});
                                    //RED.log.error("RX FROM BUS : " + input.id +" " + src + " " + dest + " " + evt)
                                    input.send(msg)
                                }
                            })
                        break;
                    }
                    case "GroupValue_Response": {
                        
                        node.nodeClients
                            .filter(input => input.notifyresponse==true)
                            .forEach(input => {
                                if (input.listenallga==true) {
                                    // Get the DPT
                                    let oGA;
                                    try {
                                        oGA=node.csv.filter(sga => sga.ga == dest)[0];
                                    } catch (error) { }
                                    
                                    // 25/10/2019 TRY TO AUTO DECODE
                                    // --------------------------------
                                    if (typeof oGA === "undefined") {
                                        let msg = buildInputMessage(src, dest, evt, rawValue, tryToFigureOutDataPointFromRawValue(rawValue,dest), "")
                                        input.setNodeStatus({ fill: "green", shape: "dot", text: "Try to decode",payload: msg.payload, GA: msg.knx.destination, dpt:"", devicename:"" });
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
                                    input.setNodeStatus({ fill: "blue", shape: "dot", text: "", payload: msg.payload, GA: input.topic, dpt:msg.knx.dpt, devicename:msg.devicename });
                                    input.send(msg)
                                }
                            })
                        break;
                    }
                    case "GroupValue_Read": {
                        
                        node.nodeClients
                            .filter(input => input.notifyreadrequest==true)
                            .forEach(input => {
                                if (input.listenallga==true) {
                                    // Get the DPT
                                    let oGA;
                                    try {
                                        oGA=node.csv.filter(sga => sga.ga == dest)[0];
                                    } catch (error) { }
                                    
                                    // 25/10/2019 TRY TO AUTO DECODE
                                    // --------------------------------
                                    if (typeof oGA === "undefined") {
                                        // 25/10/2019 from v. 1.1.11, try to decode and output a datapoint.
                                        let msg = buildInputMessage(src, dest, evt, null, tryToFigureOutDataPointFromRawValue(rawValue,dest), "")
                                        input.setNodeStatus({ fill: "green", shape: "dot", text: "Try to decode",payload: msg.payload, GA: msg.knx.destination, dpt:"", devicename:"" });
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
                                    if (input.notifyreadrequestalsorespondtobus===true) {
                                        if (typeof input.currentPayload === "undefined" || input.currentPayload === "") {
                                            setTimeout(() => {
                                                node.tcpConnection.respond(dest, input.notifyreadrequestalsorespondtobusdefaultvalueifnotinitialized, input.dpt);
                                                input.setNodeStatus({ fill: "blue", shape: "ring", text: "Read & Autorespond with default", payload: input.notifyreadrequestalsorespondtobusdefaultvalueifnotinitialized, GA: input.topic, dpt: msg.knx.dpt, devicename: "" });
                                            }, 200);
                                        } else {
                                            setTimeout(() => {
                                                node.tcpConnection.respond(dest, input.currentPayload, input.dpt);    
                                                input.setNodeStatus({ fill: "blue", shape: "ring", text: "Read & Autorespond", payload: input.currentPayload, GA: input.topic, dpt: msg.knx.dpt, devicename: "" });
                                            }, 200);
                                        }
                                    } else {
                                        input.setNodeStatus({ fill: "grey", shape: "dot", text: "Read", payload: msg.payload, GA: input.topic , dpt:msg.knx.dpt, devicename:"" });    
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
        

       

  
        // 26/10/2019 Try to figure out the datapoint type from raw value
        function tryToFigureOutDataPointFromRawValue(_rawValue) {
            // 25/10/2019 Try some Datapoints
            if (_rawValue.length == 1) {
                if (_rawValue[0].toString() == "0" || _rawValue[0].toString() == "1") {
                    return "1.001"; // True/False?
                } else {
                    return "5.001"; // Absolute Brightness ?       
                }
            } else if (_rawValue.length == 4) {
                return "14.056"; // Watt ?
            } else if (_rawValue.length == 2) {
                return "9.001"; 
            } else if (_rawValue.length == 14) {
                return "16.001"; // Text ?
            } else {
                // Dont' know, try until no errors
                let dpts =
                    Object.entries(dptlib)
                        .filter(onlyDptKeys)
                        .map(extractBaseNo)
                        .sort(sortBy("base"))
                        .reduce(toConcattedSubtypes, []);
                for (let index = 0; index < dpts.length; index++) {
                    const element = dpts[index];
                    try {
                        //dpt.value)
                        //dpt.text))
                        var dpt = dptlib.resolve(element.value);
                        if (typeof dpt !== "undefined")
                        {
                            var jsValue = dptlib.fromBuffer(_rawValue, dpt)
                            if (typeof jsValue !== "undefined") { 
                                //RED.log.info("Trying for " + dest + ". FOUND " + element.value);
                                return element.value;
                            }
                        }
                    } catch (error) {
                        
                    }                        
                }
            }
        }


         // 14/08/2019 If the node has payload same as the received telegram, return false
        checkRBEInputFromKNXBusAllowSend = (_node, _KNXTelegramPayload) => {
            if (_node.inputRBE !== true) return true;
            if (typeof _node.currentPayload === "undefined") return true;
            var curVal = _node.currentPayload.toString().toLowerCase();
            var newVal = _KNXTelegramPayload.toString().toLowerCase();
            if (curVal==="false") {
                curVal = "0";
            }
            if (curVal==="true") {
                curVal = "1";
            }
            if (newVal==="false") {
                newVal = "0";
            }
            if (newVal==="true") {
                newVal = "1";
            }
            if (curVal === newVal) {
                 return false;
            }
            return true;
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
 
        function readCSV(_csvText) {
                
            var ajsonOutput = new Array(); // Array: qui va l'output totale con i nodi per node-red
           
            if (_csvText == "") {
                RED.log.info('knxUltimate: no csv ETS found');
                return;
            } else {
                RED.log.info('knxUltimate: csv ETS found !');
                // 23/08/2019 Delete inwanted CRLF in the GA description
                let sTemp = correctCRLFInCSV(_csvText);
                
                // Read and decode the CSV in an Array containing:  "group address", "DPT", "Device Name"
                let fileGA = sTemp.split("\n");
                // Controllo se le righe dei gruppi contengono il separatore di tabulazione
                if (fileGA[0].search("\t") == -1) {
                    RED.log.error('knxUltimate: ERROR: the csv ETS file must have the tabulation as separator')
                    return;
                }
    
                var sFirstGroupName = "";
                var sSecondGroupName = "";
                var sFather="";
                for (let index = 0; index < fileGA.length; index++) {
                    var element = fileGA[index];
                    element = element.replace(/\"/g, ""); // Rimuovo le virgolette
                
                    if (element !== "") {
                        
                        // Main and secondary group names
                        if ((element.split("\t")[1].match(/-/g) || []).length == 2) {
                            // Found main group family name (Example Light Actuators)
                            sFirstGroupName = element.split("\t")[0] || "";
                            sSecondGroupName = "";
                        }
                        if ((element.split("\t")[1].match(/-/g)||[]).length==1) {
                            // Found second group family name (Example First Floor light)
                            sSecondGroupName = element.split("\t")[0] || "";
                        }
                        if(sFirstGroupName!=="" && sSecondGroupName !==""){sFather="(" + sFirstGroupName + "->" +sSecondGroupName + ") " }
                       
                        if (element.split("\t")[1].search("-") == -1 && element.split("\t")[1].search("/") !== -1) {
                            // Ho trovato una riga contenente un GA valido, cioè con 2 "/"
                            if (element.split("\t")[5] == "") {
                                RED.log.error("knxUltimate: ERROR: Datapoint not set in ETS CSV. Please set the datapoint with ETS and export the group addresses again. ->" + element.split("\t")[0] + " " + element.split("\t")[1])
                                return;
                            }
                            var DPTa = element.split("\t")[5].split("-")[1];
                            var DPTb = element.split("\t")[5].split("-")[2];
                            if (typeof DPTb == "undefined") {
                                RED.log.warn("knxUltimate: WARNING: Datapoint not fully set (there is only the first part on the left of the '.'). I applied a default .001, but please set the datapoint with ETS and export the group addresses again. ->" + element.split("\t")[0] + " " + element.split("\t")[1] + " Datapoint: " + element.split("\t")[5] );
                                DPTb = "001"; // default
                            }
                            // Trailing zeroes
                            if (DPTb.length == 1) {
                                DPTb = "00" + DPTb;
                            } else if (DPTb.length == 2) {
                                DPTb = "0" + DPTb;
                            } if (DPTb.length == 3) {
                                DPTb = "" + DPTb; // stupid, but for readability
                            }
                            ajsonOutput.push({ga:element.split("\t")[1], dpt:DPTa + "." + DPTb, devicename: sFather + element.split("\t")[0]});
                        }
                    }
                }
                
                return ajsonOutput;
            }
    
        }
    
     
        // 23/08/2019 Delete unwanted CRLF in the GA description
        function correctCRLFInCSV(_csv) {
            
            var sOut = ""; // fixed output text to return
            var sChar = "";
            var bStart = false;
            for (let index = 0; index < _csv.length; index++) {
                sChar = _csv.substr(index, 1);
                if (sChar == "\"")
                {
                    if (!bStart) {
                        bStart = true;                        
                    }else
                    {
                        bStart = false;                        
                    }
                    sOut += sChar;

                } else
                {
                    if (bStart) {
                        // i'm in the phrase, delimited by "". No CRLF should be there
                        if (sChar !== "\n" && sChar !== "\r")
                        {
                            sOut += sChar;        
                        } else
                        {
                            sOut += " "; // Where it was a CRLF, i put a space
                        }
                    } else
                    {
                        sOut += sChar; 
                    }
                    
                }
            }

            // Replace all parenthesis with []
            sOut = sOut.replace(/\(/g, "[").replace(/\)/g, "]");
            return sOut;
        }

    }

   

    RED.nodes.registerType("crestronNode-config", crestronNodeConfigNode);
}
