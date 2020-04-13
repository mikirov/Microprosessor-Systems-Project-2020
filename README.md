# NodeMCU Bluetooth led strip controller

## Intro

This project creates a led strip controller over Wi-fi and Bluetooth. This was accomplished using a NodeMCU esp8266 Wi-Fi module and HC-06 Bluetooth module. The following schematic describes how to connect the components:

![schematic](https://github.com/mikirov/Microprosessor-Systems-Project-2020/blob/master/images/diagram.png)

The web part works in the following way: 

The nodemcu creates a web server at static IP 192.168.1.143 and well-known port 80. It connects to the internet from my home network, for which I give my SSID and password. 

A get request on '/' gives us the default page. It consists of a form with a color picker and a submit button.


A post request on '/color' sends the url-encoded color parameter as a hexadecimal 3-byte sequence. The web  server interprets it and sets the color of the led strip accordingly.

[!][postman](https://github.com/mikirov/Microprosessor-Systems-Project-2020/blob/master/images/postman.PNG)

resulting in:

![led-green](http://github.com/mikirov/Microprosessor-Systems-Project-2020/blob/master/images/led_green.jpg)

we can also send a post request directly from the color picker user interface:

![webpage](http://github.com/mikirov/Microprosessor-Systems-Project-2020/blob/master/images/webpage.PNG)

resulting in:

![led-blue](http://github.com/mikirov/Microprosessor-Systems-Project-2020/blob/master/images/led_blue.jpg)

==========

