from Adafruit_ADS1x15 import ADS1115
from neopixel import NeoPixel
from umodbus.client import tcp
from umodbus.constants import READ_HOLDING_REGISTERS
import time
from machine import Pin, UART, ADC
from umqtt.simple import MQTTClient

RS485_RX_PIN = Pin(21, Pin.OUT)
RS485_TX_PIN = Pin(22, Pin.OUT)

PIN = 25
NUMPIXELS = 1
BOT_TOKEN = "6910328263:AAFSaqnW6Gc9JXkonkqYAsemwAR591aEdOo"

ads1 = ADS1115()
pixels = NeoPixel(Pin(PIN), NUMPIXELS)

ssid = "FASTWEB-Ubx8JX"
password = "TGqfj9GTK4"
mqtt_server = "38.242.140.25"

client = MQTTClient("ESP32Client", mqtt_server)

def pre_transmission():
    RS485_RX_PIN.value(1)
    RS485_TX_PIN.value(1)
    time.sleep_us(500)

def post_transmission():
    RS485_RX_PIN.value(0)
    RS485_TX_PIN.value(0)
    time.sleep_us(500)

def connect_wifi():
    import network
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

def read_modbus_registers():
    try:
        result = tcp.read_input_registers(1, 3, unit=1)
        register1, register2, register3, register4 = result[0], result[1], result[2], result[3]

        payload = f'{{"register1": {register1}, "register2": {register2}, "register3": {register3}, "register4": {register4}}}'
        payload_char = payload.encode('utf-8')

        client.publish(b"norvi/BM1-ES/RS485/test", payload_char)
    except Exception as e:
        print(f"Error reading Modbus registers: {e}")

def main():
    pixels.brightness(90)
    pixels[0] = (0, 255, 0)
    pixels.write()

    connect_wifi()
    connect_mqtt()

    while True:
        read_modbus_registers()
        time.sleep(10)

if __name__ == "__main__":
    main()
