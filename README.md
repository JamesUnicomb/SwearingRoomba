# SwearingRoomba


## The Finished Product
[![IThe swearing Roomba](https://img.youtube.com/vi/M5FXmOTXk3Q/0.jpg)](https://www.youtube.com/watch?v=M5FXmOTXk3Q)

## The Parts

<p align="center">
<img src="https://github.com/JamesUnicomb/SwearingRoomba/blob/master/shield_assembled.JPG" width="240" />
</p>

<p align="center">
<img src="https://github.com/JamesUnicomb/SwearingRoomba/blob/master/shield_mega.JPG" width="240" />
</p>

<p align="center">
<img src="https://github.com/JamesUnicomb/SwearingRoomba/blob/master/prototype.JPG" width="240" />
</p>

### DFRobot Mini MP3 Player
https://www.dfrobot.com/product-1121.html

### Arduino Mega and Shield
These can be bought from just about any electronics website.

## The Code
### Drive Randomly
The robot follows a random walk with a constant forward velocity and white noise as angular acceleration.

### Swear When Bump
Continuously check if the bumper has been pressed in a while loop (dont forget to initialise the bumper before the while loop starts - sometimes the default can be true for bool and causes frustration).

When it has been pressed, do an evasive manouver - basically reverse a little, turn at some random angle, and go back to driving randomly.

### The Swear
DF Robot Mini MP3 player takes care of this: simply load your swear sounds onto an SD card and "play next" swear.
