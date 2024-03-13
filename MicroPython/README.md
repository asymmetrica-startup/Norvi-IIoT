# MicroPython FOR NORVI

- Reference --> [https://micropython.org/download/ESP32_GENERIC/]

- Thonny IDE --> [https://thonny.org/]

- How Use Thonny IDE --> [https://randomnerdtutorials.com/getting-started-thonny-micropython-python-ide-esp32-esp8266/]

- esptool.py --> [https://vixr.it/guida-allinstallazione-e-luso-di-esptool-py-in-italiano/]


Check USBSerial Ports in Linux 
```
dmesg | grep tty
```

esptool.py USE

> BACKUP FIRMWARE
```
esptool.py --port <serialport> read_flash 0x00000 0x100000 firmware.bin
esptool.py --port COMx read_flash 0x00000 0x100000 firmware.bin
esptool.py --port /dev/cu.usbserial-xxxx read_flash 0x00000 0x100000 firmware.bin
esptool.py --port /dev/ttyUSBx read_flash 0x00000 0x100000 firmware.bin
```

> ERASE FIRMWARE
```
esptool.py --port <serialport> erase_flash
esptool.py --chip esp32 --port COMx erase_flash
esptool.py --chip esp32 --port /dev/cu.usbserial-xxxx erase_flash
esptool.py --chip esp32 --port /dev/ttyUSBx erase_flash
```

> WRITE FIRMWARE
```
esptool.py --chip esp32 --port COMx --baud 460800 write_flash -z 0x1000 esp32-firmware.bin
esptool.py --chip esp32 --port /dev/cu.usbserial-xxxx --baud 460800 write_flash -z 0x1000 esp32-firmware.bin
esptool.py --chip esp32 --port /dev/ttyUSBx --baud 460800 write_flash -z 0x1000 esp32-firmware.bin
```