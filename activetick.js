var addon = require('./build/Release/NodeActivetickAddon');
var async = require('async');
var messageIds = require('./messageIds');

function NodeActivetick() {
  this.client = new addon.NodeActivetick();
  this.handlers = {};
  this.isProcessing = false;
}

NodeActivetick.prototype = {
  _consumeMessages: function () {
    async.forever( this._processMessage.bind( this ),
      function( err ) {
        console.error( err );
        throw err;
      }
    );
  },
  _processMessage: function (next) {
    var message = this.client.popMsg();
    if (message.messageId in this.handlers) {
      var handler = this.handlers[message.messageId];
      handler(message);
    }
    setTimeout(next,0);
  },
  beginProcessing: function () {
    if (!this.isProcessing) {
      this._consumeMessages();
      this.isProcessing = true;
    }
  },
  connect: function (apiKey, serverAddr, port, username, password) {
    return this.client.init( apiKey, serverAddr, port, username, password );
  }
}

exports = module.exports = {
  "NodeActivetick": NodeActivetick,
  "messageIds": messageIds
};