const express = require("express");
const http = require("http");
const WebSocket = require("ws");

const { SerialPort } = require("serialport");
const { ReadlineParser } = require("@serialport/parser-readline");

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

app.use(express.static("public"));

wss.on("connection", (ws) => {
    for (const client of wss.clients) {
        if (client === ws) continue;

        ws.on("message", (message) => {
            console.log("Broadcasting: ", message.toString());
            client.send(message.toString());
        });
    }
});

server.listen(3000, () => {
    console.log("Server running on http://localhost:3000");
});
