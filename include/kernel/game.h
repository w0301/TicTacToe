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

#ifndef GAME_H
#define GAME_H

#include "main.h"

#include <QObject>
#include <QVector>

class Player;
class PlayBoard;
class MainWindow;

class Game : public QObject {
    Q_OBJECT

    public:
        Game(QVector<Player*>, MainWindow* = NULL, int = DEFAULT_BOARD_SIZE, int = DEFAULT_TIME_LIMIT);
        virtual ~Game() { };

        // zistenie a nastavenie hraca, ktory je na rade
        void incActualPlayer();
        Player *actualPlayer() const;

        // vrati hraciu plochu
        PlayBoard *playBoard() {
            return m_playBoard;
        };

    public slots:
        void processPlayer(int, int);

    private:
        // uchovava vsetkych hracov hry
        QVector<Player*> m_players;

        // urcuje, ktory hrac je na rade
        int m_actualPlayerIndex;

        // uchovava hraciu plochu hry
        PlayBoard *m_playBoard;

        // uchovava time limit pre tah
        int m_timeLimit;
};

#endif // GAME_H
