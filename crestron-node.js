module.exports = function (RED) {
    function CrestronNode(config) {
        RED.nodes.createNode(this, config);
        var node = this;
        node.cid = config.cid || "1";
        node.ctype = config.ctype || "Digital";
        var value;
        node.on('input', function (msg) {
            value = msg.payload;
            console.log(node.ctype + ":" + node.cid + ":Value:" + value + "*");
            node.send(msg);
        });
    }
    RED.nodes.registerType("crestron-node", CrestronNode);
}