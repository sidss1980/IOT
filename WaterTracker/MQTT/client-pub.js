var mqtt    = require('mqtt');
var client  = mqtt.connect('mqtt://192.168.1.3');

client.on('connect', function () {
    client.publish('v1/devices/me/command', '11', {retain: false, qa: 1});
    client.end();
});