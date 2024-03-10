# MicroPython FOR NORVI

- Reference --> [https://micropython.org/download/ESP32_GENERIC/]

- Thonny IDE --> [https://thonny.org/]

- EspTool.py --> [https://vixr.it/guida-allinstallazione-e-luso-di-esptool-py-in-italiano/]


Check USBSerial Ports in Linux 
```
dmesg | grep tty
```

esptool.py USE

> BACKUP FIRMWARE
```
esptool.py --port <serialport> read_flash 0x00000 0x100000 firmware.bin
```

> ERASE FIRMWARE
```
esptool.py --port <serialport> erase_flash
```

> WRITE FIRMWARE
```
esptool.py --port <serialport> write_flash -fs 1MB -fm dout 0x0 firmware.bin
```