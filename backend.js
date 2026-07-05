const express = require("express");
const http = require("http");
const { Server } = require("socket.io");

const { SerialPort } = require("serialport");
const { ReadlineParser } = require("@serialport/parser-readline");

const app = express();
const server = http.createServer(app);
const io = new Server(server);

app.use(express.static("public"));

const port = new SerialPort({
    path: "COM3",
    baudRate: 57600
});

const parser = port.pipe(new ReadlineParser({ delimiter: "\r\n" }));

parser.on("data", (line) => {
    console.log(line);

    const parts = line.split(",");
    if (parts.length !== 3) return;

    io.emit("sensor", {
        sensor: Number(parts[0]),
        value: Number(parts[1]),
        category: Number(parts[2])
    });
});

server.listen(3000, () => {
    console.log("Server running on http://localhost:3000");
});