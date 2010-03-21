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

#include <QColor>

class Player {
    public:
        // enumeracie, ktore urcuju to ako
        // bude vuzerat hrac na hracej ploche
        enum PlayerShape {
            Circle,
            Cross
        };
        typedef QColor PlayerColor;

        Player(PlayerShape, PlayerColor);

        // klasicke pristupove funkcie
        int id() const {
            return m_id;
        };
        PlayerShape shape() const {
            return m_shape;
        };
        PlayerColor color() const {
            return m_color;
        };

    private:
        // kazdy hrac bude mat unikatne ID
        int m_id;

        // IDcko bude generovane automaticky
        static int sm_idGenerator;

        // tvar a farba hraca
        PlayerShape m_shape;
        PlayerColor m_color;
};

#endif // PLAYER_H
