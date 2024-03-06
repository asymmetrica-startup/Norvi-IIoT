from machine import Pin, I2C
from neopixel import NeoPixel
from sensirion_i2c_sen5x import SensirionI2CSen5x
from ads1x15 import ADS1115
import time
import network
from umqtt.simple import MQTTClient

PIN = 25
NUMPIXELS = 1
BOT_TOKEN = "6910328263:AAFSaqnW6Gc9JXkonkqYAsemwAR591aEdOo"

ssid = "FASTWEB-Ubx8JX"
password = "TGqfj9GTK4"
mqtt_server = "38.242.140.25"

pixels = NeoPixel(Pin(PIN), NUMPIXELS)
i2c = I2C(scl=Pin(21), sda=Pin(22))
ads1 = ADS1115(i2c)
sen5x = SensirionI2CSen5x(i2c)

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

def print_module_versions():
    product_name = bytearray(32)
    sen5x.get_product_name(product_name)
    print(f"ProductName: {product_name.decode()}")

    firmware_major, firmware_minor, _, hardware_major, hardware_minor, _, _ = sen5x.get_version()
    print(f"Firmware: {firmware_major}.{firmware_minor}, Hardware: {hardware_major}.{hardware_minor}")

def print_serial_number():
    serial_number = bytearray(32)
    sen5x.get_serial_number(serial_number)
    print(f"SerialNumber: {serial_number.decode()}")

def setup():
    print_module_versions()
    print_serial_number()

    pixels.brightness(90)
    pixels[0] = (0, 0, 255)
    pixels.write()

    # Start Measurement
    sen5x.device_reset()
    sen5x.set_temperature_offset_simple(0.0)
    sen5x.start_measurement()

    connect_wifi()
    connect_mqtt()

setup()

while True:
    print("Analog 0:", ads1.read_adc(0))
    time.sleep(1)

    mass_concentration_pm1p0, mass_concentration_pm2p5, mass_concentration_pm4p0, \
    mass_concentration_pm10p0, ambient_humidity, ambient_temperature, voc_index, nox_index = sen5x.read_measured_values()

    print(f"MassConcentrationPm1p0: {mass_concentration_pm1p0}\t"
          f"MassConcentrationPm2p5: {mass_concentration_pm2p5}\t"
          f"MassConcentrationPm4p0: {mass_concentration_pm4p0}\t"
          f"MassConcentrationPm10p0: {mass_concentration_pm10p0}\t"
          f"AmbientHumidity: {ambient_humidity if not math.isnan(ambient_humidity) else 'n/a'}\t"
          f"AmbientTemperature: {ambient_temperature if not math.isnan(ambient_temperature) else 'n/a'}\t"
          f"VocIndex: {voc_index if not math.isnan(voc_index) else 'n/a'}\t"
          f"NoxIndex: {nox_index if not math.isnan(nox_index) else 'n/a'}")

    payload = {
        "MassConcentrationPm1p0": mass_concentration_pm1p0,
        "MassConcentrationPm2p5": mass_concentration_pm2p5,
        "MassConcentrationPm4p0": mass_concentration_pm4p0,
        "MassConcentrationPm10p0": mass_concentration_pm10p0,
        "AmbientHumidity": ambient_humidity,
        "AmbientTemperature": ambient_temperature
    }

    payload_str = ujson.dumps(payload)
    client.publish(b"norvi/BM1-ES/sensirion/SEN55/test", payload_str)
    client.check_msg()
    time.sleep(10)
