let net = require('net')
let server = new net.Server()

server.listen(8888)

server.on('connection', function (socket) {
//  process.exit(-1)
//  socket.destroy()
})
