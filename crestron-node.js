module.exports = function (RED) {


    function TCPServerNode(config) {
        RED.nodes.createNode(this, config);
        this.host = config.host;
        this.port = config.port;
        this.reconnect = config.reconnect;
        this.reconnecttimeout = config.reconnecttimeout;
        this.crestronconn = null;
        
        this._state = 'disconnected';
        
        let Reconnect = require('node-net-reconnect/src/index.js')
        
        let node = this;
        
        //@ts-
        let consettings = {
          host: node.host,
          port: node.port,
        };
        
        let recon;
    
        node.initializeModbusTCPConnection = function(socket, onConnect,handler) {
          timestamplog( `Connecting to tcpserver at ${node.host}:${node.port}`);
    
          if (Number(node.reconnecttimeout) > 0) {
            consettings.autoReconnect = true;
            consettings.reconnectTimeout = Number(node.reconnecttimeout) * 1000;
            consettings.retryAlways = true;
            consettings.retryTime = Number(node.reconnecttimeout) * 1000;
          }
          
          node.crestronconn = new net.Socket();
          
          const _onConnectEvent = () => {
            debug(`socket connected to ${socket.remoteAddress}:${socket.remotePort}`);
            debug(`socket connected from ${socket.localAddress}:${socket.localPort}`)
            
            // Only node >= 9.11.0 will emit a ready, so force a 
            // ready on connect for earlier releases.
    
            if (compver(process.versions.node,'9.11.0') >= 0){
              this._state = 'connected';
            }
            else{
              this._state = 'ready';
            }
    
          }
          
          const _onReadyEvent = () => {
            // We only get a 'ready' emitted for 
            // version 9.11.0 of node and higher
            this._state = 'ready';
            debug('socket ready');
          }
    
          const _onCloseEvent = (hadError) => {
            debug('socket closed. HadError = ', hadError);
            this._state = 'disconnected';
          }
          
          const _onErrorEvent = (err) => {
            node.error(`socket error: ${err.name}: ${err.message}`)
            debug(`socket error: ${err.name}: ${err.message}`)
            this._state = 'error';
            socket.destroy();
            //socket.connect(consettings);
          }
          
          
          const _onTimeoutEvent = () => {
            node.warn('socket timeout');
            debug('socket timeout');
          }
            
          socket.on('connect', _onConnectEvent);
          socket.on('ready', _onReadyEvent);
          socket.on('close', _onCloseEvent);
          socket.on('error', _onErrorEvent);
          socket.on('timeout', _onTimeoutEvent );
        
          reconn = new Reconnect(socket,consettings);
        
          socket.connect(consettings);
    
          handler(node.crestronconn);
    
          node.on("close", function() {
            timestamplog(`Disconnecting from modbustcp slave at ${socket.remoteAddress}:${socket.remotePort}`);
            socket.removeListener('connect', _onConnectEvent);
            socket.removeListener('ready', _onReadyEvent);
            socket.removeListener('close', _onCloseEvent);
            socket.removeListener('error', _onErrorEvent);
            socket.removeListener('timeout', _onTimeoutEvent );
            reconn.end();
          });
      
    
        };
    
        node.getState = function() {
          return this._state;
        }
    
      }

    function CrestronNode(config) {
        RED.nodes.createNode(this, config);
        var node = this;
        node.cid = config.cid || "1";
        node.ctype = config.ctype || "Digital";
        node.host = config.host;
        node.port = config.port;
        node.reconnect = config.reconnect;
        node.reconnecttimeout = config.reconnecttimeout;
        node.tcpconn = null;
        node._state = 'disconnected';
        let consettings = {
            host: node.host,
            port: node.port,
        };
        var value;
        node.on('input', function (msg) {
            value = msg.payload;
            console.log(node.ctype + ":" + node.cid + ":Value:" + value + "*");
            node.send(msg);
        });


        

        
    }
}