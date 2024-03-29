/*!
*   This program is free software; you can redistribute it and/or
*   modify it under the terms of the GNU General Public License as
*   published by the Free Software Foundation; either version 3 of the
*   License, or (at your option) any later version.

*   This program is distributed in the hope that it will be useful, but
*   WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*   General Public License for more details:
*   http://www.gnu.org/licenses/gpl.txt
*/

#ifndef MAIN_H
#define MAIN_H

// includovanie suboru s build konfiguraciou
#include "config.h"

#define DEFAULT_PLAYERS_COUNT 2
#define DEFAULT_BOARD_SIZE 20
#define DEFAULT_WIN_STONES 5
#define DEFAULT_WINANIM_TIME 5000
#define DEFAULT_TIME_LIMIT 0

// minimalne a maximalne hodnoty
#define MINIMUM_PLAYERS_COUNT 2
#define MINIMUM_BOARD_SIZE 3
#define MAXIMUM_BOARD_SIZE 50
#define MINIMUM_WIN_SIGNS_COUNT MINIMUM_BOARD_SIZE

// pre update plochy - vpodstate potrebne iba pre sietovych hracov
#define CLEAR -100
#define WHOLE -1

#endif // MAIN_H
