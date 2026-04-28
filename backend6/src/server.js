import http from "http";
import app from "./";

const server = http.createServer(app);

server.listen(3000, "0.0.0.0", async () => {
  console.log("dziala")
});
