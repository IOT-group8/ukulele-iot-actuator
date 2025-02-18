const mqtt = require('mqtt');

// Log that the MQTT client is starting
console.log("Starting MQTT client...");

// Connect to the MQTT broker (replace with your broker's IP)
const client = mqtt.connect('mqtt://192.168.189.69:1883');

// When connected, log success and subscribe to a topic
client.on('connect', () => {
  console.log('✅ Connected to MQTT broker.');

  // Subscribe to a topic
  client.subscribe('sensor/raw', (err) => {
    if (!err) {
      console.log('✅ Successfully subscribed to sensor/raw.');
    } else {
      console.error('❌ Subscription error:', err);
    }
  });

  // Publish "Hello Gandu" to the topic "sensor/processed"
  client.publish('sensor/processed', "Hello Gandu", (err) => {
    if (!err) {
      console.log('📤 Published: Hello Gandu');
    } else {
      console.error('❌ Publish error:', err);
    }
  });
});

// Event listeners for better logging
client.on('reconnect', () => console.log('🔄 Reconnecting to broker...'));
client.on('offline', () => console.log('⚠️ MQTT client is offline.'));
client.on('close', () => console.log('🔌 Connection closed.'));
client.on('error', (err) => console.error('❌ MQTT error:', err));

// Listen for messages on the subscribed topic
client.on('message', (topic, message) => {
  console.log(`📩 Received message on ${topic}: ${message.toString()}`);
});
