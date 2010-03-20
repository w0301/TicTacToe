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

#include <QtGui>

PlayBoard::PlayBoard(QWidget *parent, int boardSize) :
        QWidget(parent), m_size(boardSize) {
    m_board = new int*[m_size];
    for(int i = 0; i != m_size; i++) {
        *(m_board + i) = new int[m_size];
    }
}

void PlayBoard::paintEvent(QPaintEvent*) {
    QPainter painter(this);

    // budeme carovat s velkostou widgetu, aby sme
    // nastavili velkost 1 stvorceka
    int s = qMin(width() / m_size, height() / m_size);

    // postara sa o vycentrovanie plochy na widgete
    int fromTop = (height() - s*m_size) / 2;
    int fromRight = (width() - s*m_size) / 2;

    for(int y = 0; y != m_size; y++) {
        for(int x = 0; x != m_size; x++) {
            // vykreslenie prazdneho stvorceka
            painter.drawRect(x*s + fromRight, y*s + fromTop, s, s);

            // tu pride vykreslenie kruzkov, krizikov atd.
        }
    }
}
