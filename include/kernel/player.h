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

#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"

#include <QColor>
#include <QPixmap>
#include <QObject>

class QWidget;

class Player : public QObject {
    Q_OBJECT

    public:
        // enumeracie, ktore urcuju to ako
        // bude vyzerat hrac na hracej ploche
        enum PlayerShape {
            Circle,
            Cross
        };

        Player(QObject*, PlayerShape, QColor);
        virtual ~Player() { };

        // klasicke pristupove funkcie
        int id() const {
            return m_id;
        };
        PlayerShape shape() const {
            return m_shape;
        };
        QColor color() const {
            return m_color;
        };

        // vrati true, ked je hrac ovladany pocitacom
        // reimplementuje sa v ComputerPlayer
        virtual bool isComputer() const {
            return false;
        };

        // vrati true, ked je hrac sietovi
        virtual bool isNetwork() const {
            return false;
        };

        // zisti ci je hrac obycajny - zivy a lokalny
        bool isOrdinary() const {
            return !isComputer() && !isNetwork();
        };

        // vrati obrazok pre hracov kruzok, krizik atd.
        QPixmap playerToe(const QWidget*, QPoint, int);

    public slots:
        virtual void processMove(int = -1, int = -1);

    signals:
        // signal je poslany, ked aktualny hrac dava kamen na plochu
        void moving(int, int);

    private:
        // kazdy hrac bude mat unikatne ID
        int m_id;

        // IDcko bude generovane automaticky
        static int sm_idGenerator;

        // tvar a farba hraca
        PlayerShape m_shape;
        QColor m_color;
};

#endif // PLAYER_H
