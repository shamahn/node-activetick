var addon = require('./build/Release/NodeActivetickAddon');
var async = require('async');
var messageIds = require('./messageIds');
var RateLimiter = require("limiter").RateLimiter;

function NodeActivetick() {
  this.client = new addon.NodeActivetick();
  this.limiter = new RateLimiter(1000,'second');
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

  /**
    * Returns connected session handle
    * @memberof NodeActivetick
    */
  getSessionHandle: function () {
    this.doAction( function () {
      return this.client.getSessionHandle();
    });
  },

  /**
    * Closes all outstanding requests
    * @memberof NodeActivetick
    */
  closeAllATRequests: function () {
    this.doAction( function () {
      this.client.closeAllATRequests();
    });
  },

  /**
    * Closes specific request, given an request handle
    * @memberof NodeActivetick
    * @param {int} reqId - Request handle
    */
  closeATRequest: function (reqId) {
    this.doAction( function () {
      this.client.closeATRequest(reqId);
    });
  },

  /**
    * Call this function to request bar history data
    * See import/inclde/ActiveTickServerAPI/ActiveTickServerRequestor.h
    * You must pass an object of one of three types. 'timeout' parameter within
    * the object req is optional.
    * @memberof NodeActivetick
    * @name sendATBarHistoryDbRequest
    * @param {Object[]} req -
    * @param {string} req[].symbol - Symbol name
    * @param {string} req[].barHistoryType - History type can be either
    *   "BarHistoryIntraday", "BarHistoryDaily", or "BarHistoryWeekly"
    * @param {int} req[].intradayMinuteCompression - Size of the bars in
    *   minutes
    * @param {string} req[].beginDateTime - Date formatted in "YYYY-MM-DD"
    * @param {string} req[].endDateTime - Date formatted in "YYYY-MM-DD"
    * @param {int} req[].[timeout] - Timeout in milliseconds
    */
  /**
    * @memberof NodeActivetick
    * @name sendATBarHistoryDbRequest^2
    * @param {Object[]} req -
    * @param {string} req[].symbol - Symbol name
    * @param {string} req[].barHistoryType - History type can be either
    *   "BarHistoryIntraday", "BarHistoryDaily", or "BarHistoryWeekly"
    * @param {int} req[].intradayMinuteCompression - Size of the bars in
    *   minutes
    * @param {int} req[].recordsWanted - Number of records wanted from the
    *   query
    * @param {int} req[].[timeout] - Timeout in milliseconds
    */
  /**
    * @memberof NodeActivetick
    * @name sendATBarHistoryDbRequest^3
    * @param {Object[]} req -
    * @param {string} req[].symbol - Symbol name
    * @param {string} req[].barHistoryType - History type can be either
    *   "BarHistoryIntraday", "BarHistoryDaily", or "BarHistoryWeekly"
    * @param {int} req[].intradayMinuteCompression - Size of the bars in
    *   minutes
    * @param {string} req[].beginDateTime - Date formatted in "YYYY-MM-DD"
    * @param {int} req[].recordsWanted - Number of records wanted from the
    *   query
    * @param {string} req[].cursorType - Cursor type can be "CursorForward" or
    *   "CursorBackward"
    * @param {int} req[].[timeout] - Timeout in milliseconds
    */
  sendATBarHistoryDbRequest: function ( req ) {
    this.doAction( function () {
      if ( 'beginDateTime' in req && 'endDateTime' req ) {
        if ( !('timeout' in req) ) {
          return this.client.sendATBarHistoryRequest( req.symbol,
                                               req.barHistoryType,
                                               req.intradayMinuteCompression,
                                               req.beginDateTime,
                                               req.endDateTime );
        } else {
          return this.client.sendATBarHistoryRequest( req.symbol,
                                               req.barHistoryType,
                                               req.intradayMinuteCompression,
                                               req.beginDateTime,
                                               req.endDateTime,
                                               req.timeout );
        }
      } else if ( 'recordsWanted' in req && !('beginDateTime' in req) ) {
        if ( !('timeout' in req) ) {
          return this.client.sendATBarHistoryRequest( req.symbol,
                                               req.barHistoryType,
                                               req.intradayMinuteCompression,
                                               req.recordsWanted );
        } else {
          return this.client.sendATBarHistoryRequest( req.symbol,
                                               req.barHistoryType,
                                               req.intradayMinuteCompression,
                                               req.recordsWanted,
                                               req.timeout );
        }
      } else if ( 'beginDateTime' in req && 'recordsWanted' in req ) {
        if ( !('timeout' in req) ) {
          return this.client.sendATBarHistoryRequest( req.symbol,
                                               req.barHistoryType,
                                               req.intradayMinuteCompression,
                                               req.beginDateTime,
                                               req.recordsWanted,
                                               req.cursorType );
        } else {
          return this.client.sendATBarHistoryRequest( req.symbol,
                                               req.barHistoryType,
                                               req.intradayMinuteCompression,
                                               req.beginDateTime,
                                               req.recordsWanted,
                                               req.cursorType,
                                               req.timeout );
        }
      }
    });
  },

  connect: function (apiKey, serverAddr, port, username, password) {
    return this.client.init( apiKey, serverAddr, port, username, password );
  },

  doAction: function (action) {
    this.limiter.removeTokens(1, function (err, remainingRequests) {
      action.bind(this)();
    }.bind(this));
  }
}

exports = module.exports = {
  "NodeActivetick": NodeActivetick,
  "messageIds": messageIds
};
