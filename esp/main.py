from fastapi import FastAPI, Request, Form
from fastapi.responses import HTMLResponse, RedirectResponse
from fastapi.templating import Jinja2Templates
import paho.mqtt.client as mqtt

app = FastAPI()
templates = Jinja2Templates(directory="templates")

broker = "broker.hivemq.com"
topic = "esp32/led"

mqtt_client = mqtt.Client()
mqtt_client.connect(broker, 1883)

@app.get("/", response_class=HTMLResponse)
async def home(request: Request):
    return templates.TemplateResponse("index.html", {"request": request})

@app.post("/control")
async def control_led(value: str = Form(...)):
    if value in ["0", "1"]:
        mqtt_client.publish(topic, value)
    return RedirectResponse("/", status_code=303)
