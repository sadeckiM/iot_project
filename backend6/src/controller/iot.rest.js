import { Router } from "express";
import { addMeasurementData, getMeasurementData } from "../database/mock.connector.js";
import { broadcastMeasurementData } from "./iot.socket.js";

const router = Router();

router.get("/data", async (req, res) => {
  // console.log({ ...req.query }),
  // console.log({ ...req.body })
  const data = await getMeasurementData();

  res.status(200).json({ status: "ok", data: data });
});

router.post("/data", async (req, res) => {
  // console.log({ ...req.query }),
  // console.log({ ...req.body })
  const data = await database.addMeasurementData(req.body);
  broadcastMeasurementData(data);


  res.status(200).json({ status: "ok" })
});

export default router;
