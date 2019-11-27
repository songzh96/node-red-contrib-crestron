let Reconnect = require('../src/index.js')
let net = require('net')
let socket = new net.Socket()
let options = { 'host': process.argv[2], 'port': process.argv[3], 'retryAlways': true }
let recon = Reconnect.apply(socket, options)

socket.setTimeout(1000)

socket.connect(options)

let timeout
let counter = 0
socket.on('connect', function () {
  timeout = setTimeout(function () {
    counter += 1
    if (counter === 2) {
      recon.end()
      return
    }
    socket.end()
  }, 10000)
})

socket.on('close', function () {
  clearTimeout(timeout)
})

