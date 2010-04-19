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

#include "ui/playersign.h"
#include "ui/playercreator.h"

#include <QPainter>

// PlayerSignRegistrator class
PlayerSignRegistrator::SignsList PlayerSignRegistrator::sm_allSigns;

PlayerSignRegistrator::PlayerSignRegistrator(PlayerSignConstructor cnt) {
    sm_allSigns.append( qMakePair(0, cnt) );
}

bool PlayerSignRegistrator::hasOverloadSign() {
    for(SignsList::const_iterator i = list().begin(); i != list().end(); i++) {
        if(i->first > 1) {
            return true;
        }
    }
    return false;
}

void PlayerSignRegistrator::resetSignsOwners() {
    for(SignsList::iterator i = list().begin(); i != list().end(); i++) {
        i->first = 0;
    }
}

// PlayerSign class
QPixmap PlayerSign::signPixmap(const QWidget* board, QPoint startAt, int size) {
    QPixmap ret(size, size);
    if(board != NULL) {
        ret.fill(board, startAt);
    }
    else {
        ret.fill(Qt::white);
    }
    return ret;
}

//| PaintedPlayerSign class
PaintedPlayerSign::PaintedPlayerSign(const QColor& col) :
        m_color(col) {

}

PaintedPlayerSign::PaintedPlayerSign(const PaintedPlayerSign& from) :
        m_color(from.color()) {

}

PaintedPlayerSign& PaintedPlayerSign::operator= (const PaintedPlayerSign& from) {
    if(&from != this) {
        setColor( from.color() );
    }
    return *this;
}

QPixmap PaintedPlayerSign::signPixmap(const QWidget* board, QPoint startAt, int size) {
    return PlayerSign::signPixmap(board, startAt, size);
}

//| CirclePlayerSign class
PlayerSignRegistrator CirclePlayerSign::sm_register(&CirclePlayerSign::createSign);

CirclePlayerSign::CirclePlayerSign(const QColor& col) :
        PaintedPlayerSign(col) {

}

CirclePlayerSign::CirclePlayerSign(const CirclePlayerSign& from) :
        PaintedPlayerSign(from) {

}

CirclePlayerSign& CirclePlayerSign::operator= (const CirclePlayerSign& from) {
    PaintedPlayerSign::operator= (from);
    return *this;
}

QPixmap CirclePlayerSign::signPixmap(const QWidget *board, QPoint startAt, int size) {
    // nastavenie kresliacej plochy
    QPixmap ret = PaintedPlayerSign::signPixmap(board, startAt, size);
    QPainter painter(&ret);
    QPen newPen(color());
    newPen.setWidth(3);
    painter.setPen(newPen);

    // nakreslenie kruzku presne do stredu stvorceka
    painter.drawEllipse(0, 0, size - 2, size - 2);

    return ret;
}

//| CrossPlayerSign class
PlayerSignRegistrator CrossPlayerSign::sm_register(&CrossPlayerSign::createSign);

CrossPlayerSign::CrossPlayerSign(const QColor& col) :
        PaintedPlayerSign(col) {

}

CrossPlayerSign::CrossPlayerSign(const CrossPlayerSign& from) :
        PaintedPlayerSign(from) {

}

CrossPlayerSign& CrossPlayerSign::operator= (const CrossPlayerSign& from) {
    PaintedPlayerSign::operator= (from);
    return *this;
}

QPixmap CrossPlayerSign::signPixmap(const QWidget *board, QPoint startAt, int size) {
    // nastavenie kresliacej plochy
    QPixmap ret = PaintedPlayerSign::signPixmap(board, startAt, size);
    QPainter painter(&ret);
    QPen newPen(color());
    newPen.setWidth(3);
    painter.setPen(newPen);

    // nakresli krizik na stvorcek
    painter.drawLine(0, 0, size, size);
    painter.drawLine(size, 0, 0, size);

    return ret;
}

//| TrinaglePlayerSign class
PlayerSignRegistrator TrinaglePlayerSign::sm_register(&TrinaglePlayerSign::createSign);

TrinaglePlayerSign::TrinaglePlayerSign(const QColor& col) :
        PaintedPlayerSign(col) {

}

TrinaglePlayerSign::TrinaglePlayerSign(const TrinaglePlayerSign& from) :
        PaintedPlayerSign(from) {

}

TrinaglePlayerSign& TrinaglePlayerSign::operator= (const TrinaglePlayerSign& from) {
    PaintedPlayerSign::operator= (from);
    return *this;
}

QPixmap TrinaglePlayerSign::signPixmap(const QWidget *board, QPoint startAt, int size) {
    // nastavenie kresliacej plochy
    QPixmap ret = PaintedPlayerSign::signPixmap(board, startAt, size);
    QPainter painter(&ret);
    QPen newPen(color());
    newPen.setWidth(3);
    painter.setPen(newPen);

    /// nakresli trojuholnik na stvorcek
    // lava strana
    painter.drawLine(0, size, size / 2, 0);
    // prava strana
    painter.drawLine(size, size, size / 2, 0);
    // zakladna
    painter.drawLine(0, size, size, size);

    return ret;
}

//| SquarePlayerSign class
PlayerSignRegistrator SquarePlayerSign::sm_register(&SquarePlayerSign::createSign);

SquarePlayerSign::SquarePlayerSign(const QColor& col) :
        PaintedPlayerSign(col) {

}

SquarePlayerSign::SquarePlayerSign(const SquarePlayerSign& from) :
        PaintedPlayerSign(from) {

}

SquarePlayerSign& SquarePlayerSign::operator= (const SquarePlayerSign& from) {
    PaintedPlayerSign::operator= (from);
    return *this;
}

QPixmap SquarePlayerSign::signPixmap(const QWidget *board, QPoint startAt, int size) {
    // nastavenie kresliacej plochy
    QPixmap ret = PaintedPlayerSign::signPixmap(board, startAt, size);
    QPainter painter(&ret);
    QPen newPen(color());
    newPen.setWidth(3);
    painter.setPen(newPen);

    /// nakresli stvorcek na stvorcek
    int padd = size / 4;
    painter.drawRect(0 + padd, 0 + padd, size - (2*padd), size - (2*padd));

    return ret;
}
