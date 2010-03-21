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

// funckia zapise na poziciu [x, y] hraca pl, sa mozrejme
// pokial tam nieje uz iny hrac - vrati true ked tam niekto je
// vrati false a nazapise nic
void PlayBoard::putPlayer(int x, int y) {
    Player *pl = qobject_cast<Player*>(sender());
    if(m_board[y][x] == NULL) {
        m_board[y][x] = pl;
    }
}

// prebera suradnice na widgete a podla toho zavola putPlayer
void PlayBoard::putPlayerOriental(int x, int y) {
    Player *pl = qobject_cast<Player*>(sender());
    int wh = m_size * m_sideSize;
    QRect boardRect(m_fromRight, m_fromTop, wh, wh);
    if(boardRect.contains(x, y)) {
        // prevedie suradnice v stvorceku na suradnice v poli
        // nastavi hraca
        if(m_board[y][x] == NULL) {
            m_board[y][x] = pl;
        }
    }
}

// vrati suradnice laveho horneho rohu stvorceka, ktory obsahuje [x, y]
QPoint PlayBoard::squareCoords(int x, int y) {
    int wh = m_size * m_sideSize;
    QRect boardRect(m_fromRight, m_fromTop, wh, wh);
    if(boardRect.contains(x, y)) {
        // prevedie suradnice v stvorceku na suradnice v poli
        // nastavi hraca
        return QPoint( ((x - m_fromRight) / m_sideSize) * m_sideSize, ((y - m_fromTop) / m_sideSize) * m_sideSize);
    }
    return QPoint(-1, -1);
}

// vrati x a y suradnicu (zabalenu v QPoint) v poli patraciu k predanemu bodu
QPoint PlayBoard::arrayCoords(int x, int y) {
    QPoint corner = squareCoords(x, y);
    if(corner == QPoint(-1, -1)) {
        return QPoint(-1, -1);
    }
    return QPoint(corner.x() / m_sideSize, corner.y() / m_sideSize);
}

void PlayBoard::mouseReleaseEvent(QMouseEvent *event) {
    // suradnice kliknutia resp. uvolnenia mysi
    int x = event->x();
    int y = event->y();

    QPoint arrCoords = arrayCoords(x, y);

    if(arrCoords != QPoint(-1, -1)) {
        // posle signal o tom, ze doslo ku kliknutiu
        emit squareClicked(arrCoords.x(), arrCoords.y());

        // po moznom vykresleni nasleduje prekreslenie hracej plochy
        update();
    }
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
                int squareX = (x*m_sideSize + m_fromRight) + 1;
                int squareY = (y*m_sideSize + m_fromTop) + 1;
                painter.drawPixmap(squareX, squareY, m_board[y][x]->playerToe(this, QPoint(squareX, squareY), m_sideSize));
            }
        }
    }
}
