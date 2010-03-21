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

#include <QPainter>

int Player::sm_idGenerator = 1;

// konstruktor automaticky vygeneruje ID hraca
// no nastavenie tvaru prebieha rucne
Player::Player(QObject *parent, PlayerShape shape, QColor col) :
        QObject(parent), m_id(sm_idGenerator++), m_shape(shape), m_color(col) {

}

// nakresli obrazok, ktory bude obsahovat hracov kamen - to co sa kresli na plochu
// velkost stvorca je predany
QPixmap Player::playerToe(const QWidget *board, QPoint startAt, int size) {
    QPixmap ret(size, size);
    ret.fill(board, startAt);
    QPainter painter(&ret);
    QPen newPen(color());
    newPen.setWidth(3);
    painter.setPen(newPen);

    if(shape() == Player::Circle) {
        // nakreslenie kruzku presne do stredu stvorceka
        painter.drawEllipse(0, 0, size - 2, size - 2);
    }
    else if(shape() == Player::Cross) {
        // nakresli krizik, na stvorcek
        painter.drawLine(0, 0, size, size);
        painter.drawLine(size, 0, 0, size);
    }
    return ret;
}

void Player::processMove(int arrX, int arrY) {
    // obycajny hrac proste hodi krizik na miesto
    emit moving(arrX, arrY);
}
