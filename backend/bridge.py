import json
from contextlib import asynccontextmanager
from fastapi import FastAPI
import paho.mqtt.client as mqtt
from influxdb_client.client.influxdb_client import InfluxDBClient
from influxdb_client.client.write.point import Point
from influxdb_client.client.write_api import SYNCHRONOUS
from dotenv import load_dotenv
import os

load_dotenv()

INFLUX_URL = "https://us-east-1-1.aws.cloud2.influxdata.com" 
INFLUX_ORG = "KoNaR"
INFLUX_BUCKET = "ms_sensors"

MQTT_BROKER = "uea5aa71.ala.eu-central-1.emqxsl.com"
MQTT_PORT = 8883
MQTT_USER = "esp32_mic"
MQTT_TOPIC = "sensors/audio"

INFLUX_TOKEN = os.getenv("INFLUX_TOKEN")
MQTT_PASSWORD = os.getenv("MQTT_PASSWORD")

if not INFLUX_TOKEN or not MQTT_PASSWORD:
    raise ValueError("There's no password or token. Check .env file.")

current_status = {
    "device" : "No data",
    "db" : 0.0,
    "status" : "checking for connection"
}

influx_meta = InfluxDBClient(url=INFLUX_URL, token =
                                             INFLUX_TOKEN, org = INFLUX_ORG)
write_api = influx_meta.write_api(write_options=SYNCHRONOUS)

def on_connect(client, userdata, flags, reason_code, properties=None):
    if reason_code == 0:
        print("Successfully connected to brocker.")
        client.subscribe(MQTT_TOPIC)
    else:
        print("Error connecting to MQTT")

def on_message(client, userdata, msg):
    global current_status 
    try:
        payload = json.loads(msg.payload.decode('utf-8'))
        device_id = payload.get("device", "unknown")
        db_value = payload.get("db", 0.0)
        current_status["device"] = device_id
        current_status["db"] = db_value 
        current_status["status"] = "Active"
        #Point(Nazwa, Tag, Pole, [Czas])
        point = (Point("noise_level")
                 .tag("device", device_id)
                 .field("decibels", float(db_value))
        )
        write_api.write(bucket=INFLUX_BUCKET, org=INFLUX_ORG, record=point)
    except Exception as e:
        print(f"Failed processing message: {e}")

@asynccontextmanager 
async def lifespan(app: FastAPI):
    print("Booting bridge")
    mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2) #type: ignore
    mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
    mqtt_client.tls_set()
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message 
    mqtt_client.connect(MQTT_BROKER, MQTT_PORT, 60)
    mqtt_client.loop_start()
    yield
    print("Exiting backend")
    mqtt_client.loop_stop()
    mqtt_client.disconnect()
    influx_meta.close()

app = FastAPI(lifespan=lifespan)

@app.get("/")
def read_root():
    return {"message" : "Server works properly."}

@app.get("/api/noise/current")
def get_current_noise():
    return current_status
