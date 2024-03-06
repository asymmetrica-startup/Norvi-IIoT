from machine import Pin, I2C
from neopixel import NeoPixel
import network
import time
from umqtt.simple import MQTTClient
import sensirion_i2c_sps
import ujson

PIN = 25
NUMPIXELS = 1
BOT_TOKEN = "6910328263:AAFSaqnW6Gc9JXkonkqYAsemwAR591aEdOo"

ssid = "FASTWEB-Ubx8JX"
password = "TGqfj9GTK4"
mqtt_server = "38.242.140.25"

pixels = NeoPixel(Pin(PIN), NUMPIXELS)
i2c = I2C(scl=Pin(21), sda=Pin(22))

client = MQTTClient("ESP32Client", mqtt_server)

def connect_wifi():
    sta_if = network.WLAN(network.STA_IF)
    sta_if.active(True)
    sta_if.connect(ssid, password)
    print("Connecting to WiFi", end="")
    while not sta_if.isconnected():
        print(".", end="")
        time.sleep(0.5)
    print("\nConnected to WiFi")

def connect_mqtt():
    while True:
        print("Connecting to MQTT broker...")
        try:
            client.connect()
            print("Connected to MQTT broker")
            break
        except Exception as e:
            print(f"Error connecting to MQTT broker: {e}")
            print("Retrying in 5 seconds...")
            time.sleep(5)

def setup():
    pixels.brightness(90)
    pixels[0] = (0, 0, 255)
    pixels.write()

    sps = sensirion_i2c_sps.Sps30(i2c)
    sps.start_measurement()

    connect_wifi()
    connect_mqtt()

setup()

while True:
    data_ready = 0

    while data_ready == 0:
        data_ready = sps.read_data_ready()
        if data_ready == 0:
            print("Data not ready, no new measurement available")
            time.sleep(0.1)

    m = sps.read_measurement()

    print(f"PM 1.0: {m.mc_1p0}")
    print(f"PM 2.5: {m.mc_2p5}")
    print(f"PM 4.0: {m.mc_4p0}")
    print(f"PM 10.0: {m.mc_10p0}")

    print(f"NC 0.5: {m.nc_0p5}")
    print(f"NC 1.0: {m.nc_1p0 - m.nc_0p5}")
    print(f"NC 2.5: {m.nc_2p5 - m.nc_1p0}")
    print(f"NC 4.0: {m.nc_4p0 - m.nc_2p5}")
    print(f"NC 10.0: {m.nc_10p0 - m.nc_4p0}")

    payload = {
        "NC 0.5": m.nc_0p5,
        "NC 1.0": m.nc_1p0 - m.nc_0p5,
        "NC 2.5": m.nc_2p5 - m.nc_1p0,
        "NC 4.0": m.nc_4p0 - m.nc_2p5,
        "NC 10.0": m.nc_10p0 - m.nc_4p0
    }

    payload_str = ujson.dumps(payload)
    client.publish(b"norvi/BM1-ES/sensirion/SPS30/test", payload_str)
    client.check_msg()
    time.sleep(10)
