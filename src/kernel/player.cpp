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
#include "kernel/playersign.h"

#include <QPainter>

//| Player class
int Player::sm_idGenerator = 1;

// konstruktor automaticky vygeneruje ID hraca
// no nastavenie triedy, ktora vykresli podpis prebehne rucne
Player::Player(PlayerSign *sign, QString name, QObject *parent) :
        QObject(parent), m_id(sm_idGenerator++), m_sign(sign), m_name(name) {

}

Player::~Player() {
    if(m_sign != NULL) {
        delete m_sign;
    }
}

// slot je zavolany (zvatsa rucne) ked ma aktualny hrac tahat
// jeho ulohou je tah vykonat a o tahu signalizovat signalom moving(int, int)
void Player::processMove(int arrX, int arrY) {
    // obycajny hrac proste hodi krizik na miesto
    emit moving(arrX, arrY, this);
}

// tento slot je zavolany signalom Game::squareBoardUpdated
// jeho ulohou je zabezpecit to aby sa sietova hra dozvedela o tom,
// ze nastal pohyb - sietovi hrac dava vediet svojej domovskej hre
// co sa udialala za hranicami :) - pre lokalneho hraca prazdne
void Player::updateBoard(int, int, Player*) {

}
