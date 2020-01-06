module.exports = function (RED) {
  function CrestronNode(config) {
    RED.nodes.createNode(this, config);
    var node = this;
    node.cid = config.cid || "1";
    node.ctype = config.ctype || "Digital";
    node.server = RED.nodes.getNode(config.server)


    node.setNodeStatus = ({ fill, shape, text }) => {
      if (node.icountMessageInWindow == -999) return; // Locked out, doesn't change status.
      // var dDate = new Date();
      // 30/08/2019 Display only the things selected in the config

      node.status({ fill: fill, shape: shape, text: text });
    }


    node.on('input', function (msg) {
      if (typeof msg === "undefined") return;
      if (!node.server) return;
      if (node.server.linkStatus !== "connected") {
        RED.log.error("CrestronNode: Lost link due to a connection error");
        return; // 28/11/2019 If not connected, exit
      }
      // send Correct value to crestron professor
      if (node.server) {
        let value = msg.payload;
        let tcpmsg;

        switch (node.ctype) {
          case "Digital":
            {
              if (Number(value) === 1 || Number(value) === 0) {
                // data format
                value = Number(value);
                tcpmsg = String(node.ctype) + ":" + String(node.cid) + ":Set:" + String(value) + "*";
                node.server.crestronConnection.write(tcpmsg);
              }
              else {
                RED.log.warn("please fix your value,use 0/1 or true/false");
                node.setNodeStatus({ fill: "yellow", shape: "dot", text: "ERROR DIGITAL VALUE" });
              }
              break;
            }
          case "Analog":
            {
              if (Number(value) <= 65535 && Number(value) >= 0) {
                // data format
                value = Math.round(Number(value));
                tcpmsg = String(node.ctype) + ":" + String(node.cid) + ":Set:" + String(value) + "*";
                node.server.crestronConnection.write(tcpmsg);
              } else {
                RED.log.warn("please fix your value,use 0-65535");
                node.setNodeStatus({ fill: "yellow", shape: "dot", text: "ERROR ANALOG VALUE" })
              }
              break;
            }
          case "String":
            {
              tcpmsg = String(node.ctype) + ":" + String(node.cid) + ":Set:" + String(value) + "*";
              node.server.crestronConnection.write(tcpmsg);
              break;
            }
          default: return
        }

      }
      else {
        RED.log.error("crestron connection is lost");
      }

    });

    node.on('close', function () {
      if (node.server) {
        node.server.removeClient(node)
      }
    });

    if (node.server) {
      node.server.removeClient(node);
      if (node.cid) {
        node.server.addClient(node);
      }

    }

  }
  RED.nodes.registerType("crestron-node", CrestronNode)
}