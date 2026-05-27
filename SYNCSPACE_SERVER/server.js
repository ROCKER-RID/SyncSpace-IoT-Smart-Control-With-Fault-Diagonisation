const express = require("express");
const http = require("http");
const { Server } = require("socket.io");
const mqtt = require("mqtt");
let logs = []; // 🔥 store logs here
const fs = require("fs");

// load existing logs if file exists
if (fs.existsSync("logs.json")) {
  try {
    logs = JSON.parse(fs.readFileSync("logs.json"));
  } catch (e) {
    logs = [];
  }
}
const app = express();
const server = http.createServer(app);
const io = new Server(server);

const PORT = 3000;

// ---------------- MQTT ----------------
const mqttClient = mqtt.connect("mqtt://YOUR-BROKER-ID");

mqttClient.on("connect", () => {
  console.log("✅ MQTT Connected");
  mqttClient.subscribe("syncspace/#");
});

mqttClient.on("message", (topic, message) => {
  const msg = message.toString();

  console.log("MQTT:", topic, msg);

  if (topic === "syncspace/logs") {
    try {
      const entry = JSON.parse(msg);

      // ✅ FIX timestamp
      if (entry.timestamp === "NOW" || !entry.timestamp) {
        entry.timestamp = new Date().toLocaleString();
      }

      // ✅ Ensure fields exist
      entry.level = entry.level || "INFO";
      entry.source = entry.source || "UNKNOWN";
      entry.event = entry.event || "Unknown";
      entry.current = entry.current ?? "N/A";

      // ✅ ID
      entry.id = logs.length + 1;

      // ✅ Store
      logs.unshift(entry);

      if (logs.length > 1000) logs.pop();

      // ✅ SAVE to file
      const fs = require("fs");
      fs.writeFileSync("logs.json", JSON.stringify(logs, null, 2));

      // ✅ Send structured log to UI
      io.emit("log_update", entry);

    } catch (e) {
      console.log("Invalid JSON log");
    }
  }

  // keep this also (for other data)
  io.emit("mqtt_update", { topic, msg });
});

// ---------------- SOCKET ----------------
io.on("connection", (socket) => {
  console.log("Client connected");

  // 🔥 SEND OLD LOGS
  socket.emit("log_history", logs);

  // relay control
  socket.on("relay_control", ({ relay, state }) => {
    const topic = `syncspace/relay/${relay}/cmd`;
    mqttClient.publish(topic, state);
  });

  // fan control
  socket.on("fan_control", ({ fan, speed }) => {
    const topic = `syncspace/fan/${fan}/cmd`;
    mqttClient.publish(topic, String(speed));
  });
});

// ---------------- STATIC ----------------
app.use(express.static("public"));

server.listen(PORT, () => {
  console.log(`🚀 Server running on http://localhost:${PORT}`);
});