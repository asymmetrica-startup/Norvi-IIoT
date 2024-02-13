from pymodbus.client.sync import ModbusSerialClient
import paho.mqtt.client as mqtt
import time

# Configurazioni Modbus
modbus_port = '/dev/ttyUSB0'  # Imposta la porta seriale del tuo dispositivo RS485
modbus_baudrate = 9600
modbus_timeout = 1

# Configurazioni MQTT
mqtt_broker = 'indirizzo_broker'
mqtt_port = 1883
mqtt_topic = 'norvi'

# Funzione per la connessione al broker MQTT
def on_connect(client, userdata, flags, rc):
    print("Connesso al broker MQTT con codice di ritorno: " + str(rc))

# Configura il client MQTT
mqtt_client = mqtt.Client()
mqtt_client.on_connect = on_connect

# Connessione al broker MQTT
mqtt_client.connect(mqtt_broker, mqtt_port, 60)
mqtt_client.loop_start()

# Funzione per leggere dati da Modbus e pubblicarli su MQTT
def read_and_publish():
    try:
        # Connessione al dispositivo Modbus RS485
        modbus_client = ModbusSerialClient(method='rtu', port=modbus_port, baudrate=modbus_baudrate, timeout=modbus_timeout)
        modbus_client.connect()

        # Leggi dati Modbus
        result = modbus_client.read_holding_registers(0, 10, unit=1)

        # Se la lettura ha successo, pubblica i dati su MQTT
        if result.isError():
            print("Errore nella lettura Modbus")
        else:
            data = result.registers
            payload = ",".join(map(str, data))
            mqtt_client.publish(mqtt_topic, payload)
            print("Dati pubblicati su MQTT: {}".format(payload))

    except Exception as e:
        print("Errore: {}".format(str(e)))

    finally:
        if modbus_client:
            modbus_client.close()

# Loop principale
while True:
    read_and_publish()
    time.sleep(1)
