/*
 * Copyright (c) 2018 PHYTEC Messtechnik GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr.h>
#include <device.h>
#include <display/cfb.h>
#include <stdio.h>

#define DISPLAY_DRIVER      DT_INST_0_SOLOMON_SSD1306FB_LABEL

static struct device *dev;
static u16_t rows;
static u8_t ppt;
static u8_t font_width;
static u8_t font_height;

#define SELECTED_FONT_INDEX  4  // perhaps make this a config parameter

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void display_play(void)
{
    while (1) {

        for (int i = 0; i < rows; i++) {

#if 1
            switch (i) {
                case 0:
                    cfb_print(dev, " average", 0, i * ppt);
                    break;
                case 1:
                    cfb_print(dev, " good", 0, i * ppt);
                    break;
                case 2:
                    cfb_print(dev, " better", 0, i * ppt);
                    break;
                case 3:
                    cfb_print(dev, " best", 0, i * ppt);
                    break;
                default:
                    break;
            }
#else
            if (cfb_print(dev, "0123456789mMgj!\"§$%&/()=", 0, i * ppt)) {
                printk("Failed to print a string\n");
                continue;
            }
#endif
            cfb_framebuffer_finalize(dev);

            k_sleep(K_MSEC(1000));
        }

        cfb_framebuffer_clear(dev, false);

    }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void display_init(void)
{
    dev = device_get_binding(DISPLAY_DRIVER);

    if (dev == NULL) {
        printk("Device not found\n");
        return;
    }

    if (display_set_pixel_format(dev, PIXEL_FORMAT_MONO10) != 0) {
        printk("Failed to set required pixel format\n");
        return;
    }

    printk("initialized %s\n", DISPLAY_DRIVER);

    if (cfb_framebuffer_init(dev)) {
        printk("Framebuffer initialization failed!\n");
        return;
    }

    cfb_framebuffer_clear(dev, true);

    display_blanking_off(dev);

    rows = cfb_get_display_parameter(dev, CFB_DISPLAY_ROWS);
    ppt  = cfb_get_display_parameter(dev, CFB_DISPLAY_PPT);

    int num_fonts = cfb_get_numof_fonts(dev);

    for (int idx = 0; idx < num_fonts; idx++) {

        cfb_get_font_size(dev, idx, &font_width, &font_height);

        printk("index[%d] font width %d, font height %d\n",
            idx, font_width, font_height);
    }

    cfb_framebuffer_set_font(dev, SELECTED_FONT_INDEX);

    printk("selected font: index[%d]\n", SELECTED_FONT_INDEX);

    cfb_framebuffer_invert(dev);  // white on black

    printk("x_res %d, y_res %d, ppt %d, rows %d, cols %d\n",
           cfb_get_display_parameter(dev, CFB_DISPLAY_WIDTH),
           cfb_get_display_parameter(dev, CFB_DISPLAY_HEIGH),
           ppt,
           rows,
           cfb_get_display_parameter(dev, CFB_DISPLAY_COLS));
}