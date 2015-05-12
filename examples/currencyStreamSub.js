var addon = require('../activetick');
var messageIds = addon.messageIds;

var api = new addon.NodeActivetick();


var activetickCb = function (data) {
  console.log(JSON.stringify(data,null,2));
};

Object.keys(messageIds).forEach(function (messageId) {
  api.handlers[messageId] = activetickCb;
});

api.handlers["ATLoginResponse"] = function ( data ) {
  console.log( JSON.stringify(data, null, 2) );
  var req = {};
  req.symbols = "#USD/CHF,#USD/JPY,#EUR/USD,#EUR/GBP";
  req.requestType = "StreamRequestSubscribe";
  var reqNum = api.sendATQuoteStreamRequest(req);

  console.log(reqNum);
  
};

var connected = api.connect( process.env.ATAPIKEY,
                             "activetick1.activetick.com",
                             5000,
                             process.env.ATUID,
                             process.env.ATPWD );

if (connected) {
  api.beginProcessing();
}
