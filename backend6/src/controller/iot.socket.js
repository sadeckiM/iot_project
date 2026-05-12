import { Server } from "socket.io";

let io;

export const initWebSocket = async (httpServer) => {
  io = new Server(httpServer, {
    cors: {
      origin: "*",
      methods: ["GET", "POST"]
    }
  });
};

export const broadcastMeasurementData = (data) => {
  io.emit("measurement", data);
}
