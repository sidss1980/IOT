var mqtt    = require('mqtt');
var client  = mqtt.connect('mqtt://192.168.1.3');

client.on('connect', function () {
    client.subscribe('v1/devices/flow/1');

    client.on('message', function (topic, message) {
        console.log(message.toString());
        //client.end();
    });
});