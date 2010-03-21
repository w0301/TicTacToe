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

#include "main.h"
#include "kernel/player.h"

int Player::sm_idGenerator = 1;

// konstruktor automaticky vygeneruje ID hraca
// no nastavenie tvaru prebieha rucne
Player::Player(PlayerShape shape, PlayerColor col) :
        m_id(sm_idGenerator++), m_shape(shape), m_color(col) {

}
