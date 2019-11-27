# Reconnection Handler for TCP Socket Connections in node.js

This module will help you reconnect a TCP socket connection if the connection is lost. 

When you want to stay connected with a TCP server you need to implement a reconnect logic. This module will keep your connection online if possible. For example, if you want to connect to a service that's not online yet, or when someone pulls the plug.

# Example

```
    let socket = net require('net').Socket()
    let options = { 
      'host' : 'somehost', 
      'port' : someport,
      'retryTime' : 1000, // 1s for every retry
      'retryAlways' : true // retry even if the connection was closed on purpose
    }
    let Reconnect = require('node-net-reconnect')
    let recon = new Reconnect(socket, options)

    socket.connect(options)

    socket.on('connect', function () {

      /* if you enabled retryAlways, a call to 
         socket.end() will reconnect the client.
         In that case you need to close the connection
         through the recon.end() method. */
      setTimeout(function () {
        recon.end()
      }, 10000)

    })
````

# LICENCE

Copyright (C) 2017 Stefan Poeter (Stefan.Poeter[at]cloud-automation.de)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.    

    
