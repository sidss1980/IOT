var mqtt    = require('mqtt');
var client  = mqtt.connect('mqtt://10.10.11.197');

client.on('connect', function () {
    client.publish('v1/devices/flow/1', '11', {retain: false, qa: 1});
    client.end();
});