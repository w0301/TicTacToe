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
#include "ui/playboard.h"
#include "kernel/player.h"

#include <QtGui>

PlayBoard::PlayBoard(QWidget *parent, int boardSize) :
        QWidget(parent), m_size(boardSize), m_sideSize(0), m_fromRight(0), m_fromTop(0) {
    m_board.resize(m_size);
    for(int i = 0; i != m_size; i++) {
        m_board[i].resize(m_size);
    }
}

// destruktor
PlayBoard::~PlayBoard() {

}

// funckia zapise na poziciu [x, y] hraca pl, sa mozrejme
// pokial tam nieje uz iny hrac - vrati true ked tam niekto je
// vrati false a nazapise nic
bool PlayBoard::putPlayer(int x, int y, Player *pl) {
    if(m_board[y][x] == NULL) {
        m_board[y][x] = pl;
        return true;
    }
    return false;
}

// prebera suradnice na widgete a podla toho zavola putPlayer
bool PlayBoard::putPlayerOriental(int x, int y, Player *pl) {
    int wh = m_size * m_sideSize;
    QRect boardRect(m_fromRight, m_fromTop, wh, wh);
    if(boardRect.contains(x, y)) {
        // prevedie suradnice v stvorceku na suradnice v poli
        // nastavi hraca
        return putPlayer( (x - m_fromRight) / m_sideSize,
                          (y - m_fromTop) / m_sideSize, pl);
    }
    return false;
}

// vrati suradnice prave horneho rohu stvorceka
QPoint PlayBoard::squareCoords(int x, int y) {
    int wh = m_size * m_sideSize;
    QRect boardRect(m_fromRight, m_fromTop, wh, wh);
    if(boardRect.contains(x, y)) {
        // prevedie suradnice v stvorceku na suradnice v poli
        // nastavi hraca
        return QPoint( ((x - m_fromRight) / m_sideSize) * m_sideSize, ((y - m_fromTop) / m_sideSize) * m_sideSize);
    }
    return QPoint(0, 0);
}

Player pl(Player::Circle, QColor(255, 0, 0));

void PlayBoard::mouseReleaseEvent(QMouseEvent *event) {
    // suradnice kliknutia resp. uvolnenia mysi
    int x = event->x();
    int y = event->y();

    // vykreslenie - o toto sa bude starat dalsia trieda, ktora
    // moze suplovat pocitac ako hraca alebo pripojit sietoveho hraca
    putPlayerOriental(x, y, &pl);

    // po vykresleni nasleduje prekreslenie postihnutej casti hracej plochy
    update( QRegion( QRect( squareCoords(x, y), QPoint(x + m_sideSize, y + m_sideSize) ) ) );
}

void PlayBoard::resizeEvent(QResizeEvent*) {
    // budeme carovat s velkostou widgetu, aby sme
    // nastavili velkost 1 stvorceka
    m_sideSize = qMin(width() / m_size, height() / m_size);

    // postara sa o vycentrovanie plochy na widgete
    m_fromTop = (height() - m_sideSize*m_size) / 2;
    m_fromRight = (width() - m_sideSize*m_size) / 2;
}

void PlayBoard::paintEvent(QPaintEvent*) {
    QPainter painter(this);

    for(int y = 0; y != m_size; y++) {
        for(int x = 0; x != m_size; x++) {
            // vykreslenie prazdneho stvorceka
            painter.drawRect(x*m_sideSize + m_fromRight, y*m_sideSize + m_fromTop, m_sideSize, m_sideSize);

            // tu pride vykreslenie kruzkov, krizikov atd.
            if(m_board[y][x] != NULL) {
                int squareX = (x*m_sideSize + m_fromRight);
                int squareY = (y*m_sideSize + m_fromTop);
                Player::PlayerShape shape = m_board[y][x]->shape();
                Player::PlayerColor color = m_board[y][x]->color();

                QPen oldPen = painter.pen();
                QPen newPen(color);
                newPen.setWidth(3);
                painter.setPen(newPen);
                if(shape == Player::Circle) {
                    // nakreslenie kruzku presne do stredu stvorceka
                    painter.drawEllipse( squareX + 1, squareY + 1, m_sideSize - 2, m_sideSize - 2);
                }
                else if(shape == Player::Cross) {
                    // nakresli krizik, na stvorcek
                    painter.drawLine(squareX + 2, squareY + 2, squareX + m_sideSize - 2, squareY + m_sideSize - 2);
                    painter.drawLine(squareX + m_sideSize - 2, squareY + 2, squareX + 2, squareY + m_sideSize - 2);
                }
                painter.setPen(oldPen);
            }
        }
    }
}
