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

#include <QList>
#include <QPair>
#include <QColor>
#include <QPixmap>

class PlayerSign;

// PlayerSignRegistrator class - registruje triedypodpisov
class PlayerSignRegistrator {
    public:
        typedef PlayerSign *(*PlayerSignConstructor)();
        typedef QList< QPair<bool, PlayerSignConstructor> > SignsList;

        PlayerSignRegistrator(PlayerSignConstructor);

        // registruje sign / vrati ich list
        static void enable(PlayerSignConstructor cnt, bool val) {
            sm_allSigns[sm_allSigns.indexOf(qMakePair(!val, cnt))].first = val;
        };
        static SignsList& list() {
            return sm_allSigns;
        };

    private:
        static SignsList sm_allSigns;
};

// PlayerSign class - abstraktny predok "podpisu" hraca na ploche
class PlayerSign {
    public:
        PlayerSign() { } ;
        virtual ~PlayerSign() { } ;

        // vrati pixmapu parametre - widget s ktoreho prekoporije pozadie,
        // bod s ktoreho zacne brat udaje z widgetu, velkost stvorceka widgetu
        virtual QPixmap signPixmap(const QWidget*, QPoint, int);

        // vytvori novy sign
        static PlayerSign* createSign() {
            return new PlayerSign;
        };

        // vrati true ked je podpis malovany rucne
        // pre abstraktne je false
        virtual bool isPainted() const {
            return false;
        };
};

// trieda pre rucne nakresene "podpisy hracov na ploche"
class PaintedPlayerSign : public PlayerSign {
    public:
        PaintedPlayerSign() { };
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

        // vrati true ked je podpis malovany rucne
        virtual bool isPainted() const {
            return true;
        };

        // vytvori novy sign
        static PlayerSign* createSign() {
            return new PaintedPlayerSign;
        };

    private:
        QColor m_color;
};

// trieda pre nakreslenie kruzku
class CirclePlayerSign : public PaintedPlayerSign {
    public:
        CirclePlayerSign() { };
        CirclePlayerSign(const QColor&);
        CirclePlayerSign(const CirclePlayerSign&);
        virtual ~CirclePlayerSign() { } ;

        CirclePlayerSign& operator= (const CirclePlayerSign&);

        virtual QPixmap signPixmap(const QWidget*, QPoint, int);

        // vytvori novy sign
        static PlayerSign* createSign() {
            return new CirclePlayerSign;
        };

    private:
        static PlayerSignRegistrator sm_register;
};

// trieda pre nakreslenie krizika
class CrossPlayerSign : public PaintedPlayerSign {
    public:
        CrossPlayerSign() { };
        CrossPlayerSign(const QColor&);
        CrossPlayerSign(const CrossPlayerSign&);
        virtual ~CrossPlayerSign() { } ;

        CrossPlayerSign& operator= (const CrossPlayerSign&);

        virtual QPixmap signPixmap(const QWidget*, QPoint, int);

        // vytvori novy sign
        static PlayerSign* createSign() {
            return new CrossPlayerSign;
        };

    private:
        static PlayerSignRegistrator sm_register;
};


#endif // PLAYERSIGN_H
