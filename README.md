# ssd1306_zephyr

## Overview
This project implements a SSD1306 OLED display connected to a nRF52 PCA10040 (nRF52832) board running Zephyr version 2.1.
In theory, this project could be ported to other Zephyr-supported boards, but this has not (yet) been tried.

Also this project shows how to include custom fonts.

## Hardware
See photo in *docs* directory for view of hardware configuration.
Below is a list hardware components.

 * Nordic PCA10040 Eval Board (nRF52832)
 * Adafruit 128x32 I2C OLED display (SSD1306)
 * Bread board
 * Ribbon cable with male headers. 

Wire connections as follows

 * P0.27 <--> SCL
 * P0.26 <--> SDA
 * P0.02 <--> RST  (optional)
 * VDD   <--> Vin
 * GND   <--> GND

## Software
This project was built with Zephyr 2.1.99 and selects the Nordic PCA10040 board ().  
Change the "`set(BOARD nrf52_pca10040)`" in the CMakeFile.txt for other supported boards. 

The following componets need to be configured though "make menuconfig".  

 * Drivers->i2c: I2C-master support for your board/SOC.
 * Drivers->display: Display + SSD1306
 * Subsys->fb: FrameBuffer (cfb)
 * General Kernel Options->Other Kernel Object Option: Heap Memory Pool Size

The tricky part of the software configuration seems to be in setting up the **.overlay* file and the *prj.conf* file.  This requires some knowledge about the build-configuration process for zephyr.  
Hopefully, this project can be used as an example of how to navigate though this procedure.

The difficult parts are setting up the I2C driver for the target board. In this project the nRF52 I2C support is well-known and relatively straight-forward in configuring.  Other SOCs and boards are not always so easy.

**NOTE:** The Zephyr components work together to provide basic display support across a small number of display controllers. In order to provide a common feature set, many of the SSD1306's more advanced feartures are not supported. In other words, the display + frame-buffer support is more or less constrained to line-by-line console mode operations.  Read the header file *cfb.h* for API details.

Custom fonts are provided in this project and are held in the *fonts* directory. Currently there are five fonts: three fonts taken from the zephyr (*zephyr/subsys/fb/cfb_fonts*) and two custom fonts.  

 * font10x16  zephyr-copy
 * font15x24  zephyr-copy
 * font20x32  zephyr-copy
 * font5x7 custom
 * font8x8 custom

In display.c, there is the SELECTED_FONT_INDEX which selects the font index.


## Operation
The display.c program is rather simple, in that it runs a loop which writes out four different strings to the display: see the *display_play()* function.

## JLink RTT Output
If you have RTT enabled and the RTT client running, then you should see the following output.  

```
    *** Booting Zephyr OS build zephyr-v2.1.0-581-g4e135d76a3c2  ***

    main_thread

    initialized SSD1306

    index[0] font width 10, font height 16

    index[1] font width 15, font height 24

    index[2] font width 20, font height 32

    index[3] font width 5, font height 8

    index[4] font width 8, font height 8

    selected font: index[4]

    x_res 128, y_res 32, ppt 8, rows 4, cols 128
```


