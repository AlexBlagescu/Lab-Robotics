# Infinity Mirror Clock

## Short introduction about project

Infinity Mirror Clock is my final project for the Introduction to Robotics course. The main idea is that we use one mirror, a RGB led strip and one glass with 70% reflection. We put the led strip between mirror and glass and that is how we create the infinity leds effect.

## Components

[Here](https://docs.google.com/spreadsheets/d/1Htry010sDG5Vxl1XxuDkIDsEU6a6pIBbHVVmY9l-o_E/edit#gid=779150184) you can see a detailed list about components and where you will find them in Romania.

### Hardware components list:

  1. Arduino Nano 1x
  2. Adaptor cable 1x
  3. RTC module DS1307 1x
  4. Normal button 1x
  5. Wires
  6. Breadbord
  
## How it works?

We already discussed how we can make the infinity leds effect so now let's focus on how our clock works.We have 3 colors, red green and blue. Red is representing the hours, green is representing the minutes and blue one is representing the seconds. Our RTC module is giving us the hour with 0-23 format so we need to convert it. We created a formula which help us to show the correct hour H = (HC * 5) % 60 where HC is the currently hour readed from RTC.

## Bonus features and demo video

As a bonus, i decided to add 18 cool effects which makes our clock more intersting. [Here](cooming soon) you can find a demo to see how my clock and effects works

## How to use?

You just have to plug the adaptor. You can navigate beetwen effects pressing the button.

## Used libraries

  1. [FastLED](https://github.com/FastLED/FastLED/wiki/Basic-usage) - I used this library to control the RGB led strip, is pretty cool and is really easy to use it.
  2. [DS1307RTC](https://github.com/PaulStoffregen/DS1307RTC) - I used this library to control the RTC input, again, cool and easy to use it.
