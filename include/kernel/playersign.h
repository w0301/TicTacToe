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

#ifndef PLAYERSIGN_H
#define PLAYERSIGN_H

#include <QColor>
#include <QPixmap>

// abstraktny predok "podpisu" hraca na ploche
class PlayerSign {
    public:
        PlayerSign() { } ;
        virtual ~PlayerSign() { } ;

        // vrati pixmapu parametre - widget s ktoreho prekoporije pozadie,
        // bod s ktoreho zacne brat udaje z widgetu, velkost stvorceka widgetu
        virtual QPixmap signPixmap(const QWidget*, QPoint, int);
};

// trieda pre rucne nakresene "podpisy hracov na ploche"
class PaintedPlayerSign : public PlayerSign {
    public:
        PaintedPlayerSign(const QColor&);
        PaintedPlayerSign(const PaintedPlayerSign&);
        virtual ~PaintedPlayerSign() { } ;

        PaintedPlayerSign& operator= (const PaintedPlayerSign&);

        // praca s farbou
        QColor& color() {
            return m_color;
        };
        const QColor& color() const {
            return m_color;
        };
        void setColor(const QColor& col) {
            m_color = col;
        };

        virtual QPixmap signPixmap(const QWidget*, QPoint, int);

    private:
        QColor m_color;
};

// trieda pre nakreslenie kruzku
class CirclePlayerSign : public PaintedPlayerSign {
    public:
        CirclePlayerSign(const QColor&);
        CirclePlayerSign(const CirclePlayerSign&);
        virtual ~CirclePlayerSign() { } ;

        CirclePlayerSign& operator= (const CirclePlayerSign&);

        virtual QPixmap signPixmap(const QWidget*, QPoint, int);
};

// trieda pre nakreslenie krizika
class CrossPlayerSign : public PaintedPlayerSign {
    public:
        CrossPlayerSign(const QColor&);
        CrossPlayerSign(const CrossPlayerSign&);
        virtual ~CrossPlayerSign() { } ;

        CrossPlayerSign& operator= (const CrossPlayerSign&);

        virtual QPixmap signPixmap(const QWidget*, QPoint, int);
};


#endif // PLAYERSIGN_H
