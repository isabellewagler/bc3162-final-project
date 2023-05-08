# Final Project - Bluetooth-Activated Lamp

## Project Overview
As the final project for my Creative Embedded Systems course, it was very open-ended and used as an opportunity to synthesize the topics covered over the course of the semester. In class, we learned how to connect an ESP32 via wifi connection, so for my project, I decided to expand on that by also using ESP32 Bluetooth capabilities. The goal of my project was to create a device that uses Bluetooth to automatically turn on a lamp when it detects someone with a Bluetooth device in range (i.e. smartphone or smartwatch), and to automatically turn it off when the person walks away. 

For this project, I used the following hardware items:
* ESP32
* LED Lamp
* Smart Plug-In 

## Creative Vision
Since my project is mostly software-based, I did not want the enclosure design to be a focus, but instead, I wanted it to be very subtle and hidden. Since the goal of the project is to automatically perform an action when a person is in range, I did not want the person to see the device but rather only interact with it through witnessing the lamp turn on and off. 

Since the lampshade on the lamp I used is white, I decided to create a small cardboard "shelf" inside of the lampshade for the ESP32 to sit on so it is hidden from the user.  

## Technical Description
From a hardware perspective, this device is set up by plugging the ESP32 into my laptop (a battery would have been ideal, but we did not have access to one) and setting it in the enclosure box behind the lampshade. The LED lamp is plugged into the smart plug-in, which is triggered through Bluetooth.

From a software perspective, this device is configured so that the ESP32 acts as a client, scanning for Bluetooth devices and detecting how strong the signal is between the ESP32 and the detected device through the RSSI (received signal strength indicator). If it is within a certain strength threshold, the ESP32 will trigger the lamp to be turned on via the smart plug. If the lamp is already on and the ESP32 detects that the device has moved out of the threshold range, the lamp will turn off.

The smart plug is operated through an app called "Smart Life," so I used another app called "IFTTT" in order to create the trigger for the lamp to automatically turn on and off. IFTTT allows the user to set up triggers between the internet and other applications on their device. In this case, I set up two triggers: one to turn on the lamp when the request is received, and one to turn off the lamp when another request is received. In the Arduino IDE, the internet request to execute the trigger occurs by the ESP32 using HTTP GET requests. 

## Problems Encountered Along the Way
From a software perspective, this project was very complicated because the smart plug-in is activated using an app called "Smart Life." Originally, I had hoped that when the ESP32 detected a person with a smartphone in range, it could connect to the smart plug directly to turn on the lamp. However, since this plug-in is activated through an app, I had to use another app, IFTTT, in order to create a trigger, which required a lot of extra research to figure out how to then execute the trigger via the ESP32. 

Additionally, I had originally hoped that I could configure the device to be specific to my iPhone (i.e. the lamp was only triggered to turn on or off if my specific phone was in range) rather than any Bluetooth device. However, I learned that Apple will randomize the Bluetooth addresses of their devices for privacy reasons, which made it very difficult to detect only my specific iPhone. For this reason, I decided to configure the device so that it is triggered by any Bluetooth device that is in close enough range, but that has also posed problems because living in a college dorm in NYC, there have always been multiple Bluetooth devices within range when I was testing the device that I did not have control over. If I were to have more time to work on this project, I would figure out a more practical way to trigger the lamp because even though the range trigger does work, the lamp is turning on and off every few seconds since it is constantly being triggered by the many Bluetooth devices in range. 

## Further Documentation
* [Link to Blog Post](https://medium.com/@iw2230/final-project-automatic-bluetooth-activated-lamp-d37fcb74734e)
