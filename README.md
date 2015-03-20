An ESP8266-based indicator light for http://isvhsopen.com.

![](esp-isvhsopen.jpg?raw=true)

Instructions
============

0. Wiring
---------

Wire your board according to the pinout for your model:

* [Pinout Diagrams](http://www.esp8266.com/wiki/doku.php?id=esp8266-module-family)
* [Basic Wiring](http://www.esp8266.com/wiki/doku.php?id=getting-started-with-the-esp8266#basic_wiring)

Wire `GPIO12` via a 330Ω resistor to a green LED and wire `GPIO13` via a 330Ω resistor to a red LED.


1. Install the toolchain
------------------------

There are a few ways to get the official Espressif sdk and toolchain installed, but I had the most success with `esp-open-sdk` which automatically gets all the bits and pieces you need.

 * [Linux](https://github.com/pfalcon/esp-open-sdk/)
 * [Mac OS X](https://github.com/pfalcon/esp-open-sdk/issues/11#issuecomment-67887495)
 * Windows?

Depending on where you install these things, you many need to set `XTENSA_TOOLS_ROOT`, `FW_TOOL`, `SDK_BASE` environment variables or adjust this Makefile.


2. Clone this repo
------------------

```bash
mbp:Code luke$ git clone https://github.com/vhs/esp-isvhsopen
```


3. Build it
-----------

Before building, open `user/user_main.c` and change `WIFI_SSID` and `WIFI_PASSWORD`.

```bash
mbp:esp-isvhsopen luke$ make
CC user/wifi.c
AR build/app_app.a
LD build/app.out
FW firmware/0x00000.bin
FW firmware/0x40000.bin
```


4. Flash the board
------------------

Set up a Serial/UART connection to your ESP8266, set pin 0 low, and power it up. Flash it using [esptool.py](https://github.com/themadinventor/esptool/).

You will need to use the appropriate TTY device port for your UART device. If you are using a bus pirate, use UART bridge mode.

```bash
mbp:esp-isvhsopen luke$ esptool.py \
    --port /dev/tty.usbserial-A603X4V4 \
    write_flash \
    0x00000 firmware/0x00000.bin \
    0x40000 firmware/0x40000.bin
Connecting...
Erasing flash...
Writing at 0x00007700... (100 %)
Erasing flash...
Writing at 0x00064d00... (100 %)

Leaving...
```


5. Usage
--------

When it is first powered up, both LEDs will come on, indicating that it is attempting to connect to wifi and contact http://isvhsopen.com. After a few seconds, only one LED should remain on, indicating whether VHS is open. It polls every 60 seconds.

If you keep your UART connected, you will see lots of debug output.


Credits
=======

* `Makefile` from https://github.com/leon-anavi/esp-hello-world
* `wifi.[ch]` is slightly modified from https://github.com/tuanpmt/esp_mqtt
* `httpclient.[ch]` is from https://github.com/Caerbannog/esphttpclient


License
=======

The MIT License (MIT)

Copyright (c) 2015 Luke Cyca

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
