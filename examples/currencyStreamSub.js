var addon = require('../activetick');
var messageIds = addon.messageIds;

var api = new addon.NodeActivetick();

Number.prototype.round = function(places) {
  return +(Math.round(this + "e+" + places)  + "e-" + places);
}

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

api.handlers["ATStreamQuoteUpdate"] = function ( msg ) {
  var atTime = msg.data.time;
  console.log(atTime.year + "-" + atTime.month + "-" + atTime.day + " " +
              atTime.hour + ":" + atTime.minute + ":" + atTime.second + ":" +
              atTime.milliseconds + " [QUOTE] " +
              msg.data.ATSymbol.symbolStr + " " +
              msg.data.bidPrice.price.round(msg.data.bidPrice.precision) + " " +
              msg.data.askPrice.price.round(msg.data.askPrice.precision) );
};
api.handlers["ATStreamTradeUpdate"] = function ( msg ) {
  var atTime = msg.data.time;
  console.log(atTime.year + "-" + atTime.month + "-" + atTime.day + " " +
              atTime.hour + ":" + atTime.minute + ":" + atTime.second + ":" +
              atTime.milliseconds + " [TRADE] " +
              msg.data.ATSymbol.symbolStr + " " +
              msg.data.lastPrice.price.round(msg.data.lastPrice.precision) );
};

var connected = api.connect( process.env.ATAPIKEY,
                             "activetick1.activetick.com",
                             5000,
                             process.env.ATUID,
                             process.env.ATPWD );

if (connected) {
  api.beginProcessing();
}
