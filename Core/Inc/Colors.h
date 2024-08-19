/*
 * Colors.h
 *
 *  Created on: Aug 13, 2024
 *      Author: irubt
 */

#ifndef INC_COLORS_H_
#define INC_COLORS_H_
#include "ws2812b.h"

const RGB t [] =
{
		{130, 2, 255},
		{247, 255, 0},
		{255, 90, 0},
};

const HSV R [] =
{
		 {HUE_RED, 100, 100},
		 {HUE_ORANGE, 100, 100},
		 {HUE_YELLOW, 100, 100},
		 {HUE_GREEN, 100, 100},
		 {HUE_AQUA, 100, 100},
		 {HUE_BLUE, 100, 100},
		 {HUE_PURPLE, 100, 100},
		 {HUE_PINK, 100, 100},
}; // Rainbow Palette

const HSV Ukr_flag [] =
{
		{HUE_DEEP_BLUE, 100, 100},
		{HUE_DEEP_YELLOW, 100, 100},
};// Ukrainian Flag Palette

Palette_HSV Rainbow = {R, sizeof(R) / sizeof(HSV)};
Palette_HSV Ukrainian_flag = {Ukr_flag, sizeof(Ukr_flag) / sizeof(HSV)};

Palette_RGB test = {t, sizeof(t) / sizeof(RGB)};

#endif /* INC_COLORS_H_ */
