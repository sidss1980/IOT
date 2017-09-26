var mqtt    = require('mqtt');
var client  = mqtt.connect('mqtt://10.10.11.197');

client.on('connect', function () {
    client.subscribe('v1/devices/flow/1');

    client.on('message', function (topic, message) {
        console.log(message.toString());
        //client.end();
    });
});