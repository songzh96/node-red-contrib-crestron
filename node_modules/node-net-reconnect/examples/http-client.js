let Reconnect = require('../src/index.js')
let net = require('net')
let socket = new net.Socket()
let options = { 'host': process.argv[2], 'port': process.argv[3] }

Reconnect.apply(socket, options)

socket.setTimeout(5000)

socket.connect(options)

console.log('trying to connect')
socket.on('connect', function () {
  console.log('online')
})

socket.on('close', function () {
  console.log('offline')
})

