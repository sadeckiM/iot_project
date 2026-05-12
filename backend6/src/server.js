import http from "http";
import app from "./app.js";

const server = http.createServer(app);

server.listen(3000, "0.0.0.0", async () => {
  initWebSocket(server);
  console.log("dziala")
});
