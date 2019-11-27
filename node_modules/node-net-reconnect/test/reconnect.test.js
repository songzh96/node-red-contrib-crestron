/* global describe, it, beforeEach */

let EventEmitter = require('events')
let sinon = require('sinon')
let Reconnect = require('../src/index.js')

describe('NetReconnect Tests', function () {
  let socket

  beforeEach(function () {
    socket = new EventEmitter()
    socket.connect = function () { }
    socket.end = function () { }
  })

  it('should reconnect on error', function (done) {
    let socketMock = sinon.mock(socket)
    let options = { }

    Reconnect.apply(socket, options)

    socketMock.expects('connect').withArgs(options).once()

    socket.emit('close', true)

    setTimeout(function () {
      socketMock.verify()
      done()
    }, 1100)
  })
  it('should not reconnect when no error', function (done) {
    let socketMock = sinon.mock(socket)
    let options = { }

    Reconnect.apply(socket, options)

    socketMock.expects('connect').withArgs(options).never()

    socket.emit('close', false)

    setTimeout(function () {
      socketMock.verify()
      done()
    }, 1100)
  })
  it('should reconnect when no error', function (done) {
    let socketMock = sinon.mock(socket)
    let options = { 'retryAlways': true }

    Reconnect.apply(socket, options)

    socketMock.expects('connect').withArgs(options).once()

    socket.emit('close', false)

    setTimeout(function () {
      socketMock.verify()
      done()
    }, 1100)
  })
  it('should end connection', function (done) {
    let socketMock = sinon.mock(socket)
    let options = { 'retryAlways': true }

    let recon = new Reconnect(socket, options)

    socketMock.expects('connect').withArgs(options).never()

    recon.end()
    socket.emit('close', false)

    setTimeout(function () {
      socketMock.verify()
      done()
    }, 1100)
  })
})
