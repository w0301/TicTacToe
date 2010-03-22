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
        Game(QVector<Player*>, QObject* = NULL, int = DEFAULT_BOARD_SIZE, int = DEFAULT_TIME_LIMIT);
        Game(QObject* = NULL, int = DEFAULT_BOARD_SIZE, int = DEFAULT_TIME_LIMIT);
        virtual ~Game() { };

        // zistenie a nastavenie hraca, ktory je na rade
        void incActualPlayer();
        Player *actualPlayer() const;

        // vrati a nastavi vector s hracmi
        QVector<Player*>& players() {
            return m_players;
        };
        const QVector<Player*>& players() const {
            return m_players;
        };
        void setPlayers(QVector<Player*>);

        // vrati vektor plochy a vrati a nastavi jeho velkost
        QVector< QVector<Player*> >& squareBoard() {
            return m_squareBoard;
        };
        const QVector< QVector<Player*> >& squareBoard() const {
            return m_squareBoard;
        };
        Player *square(int x, int y) const {
            return m_squareBoard[y][x];
        };
        int squareBoardSize() const {
            return m_squareCount;
        };
        void setSquareBoardSize(int);

    public slots:
        void fillSquare(int, int);
        void processPlayer(int, int);

    signals:
        // posle signal o tom, ze doslo k zmene na hracej ploche
        // ako parametre posle index zmeneneho stvorceka
        // [-1, -1] signalizuje zmenu celej plochy
        void squareBoardUpdated(int = -1, int = -1);

    private:        
        // uchovava vsetkych hracov hry
        QVector<Player*> m_players;

        // urcuje, ktory hrac je na rade
        int m_actualPlayerIndex;

        // uchovava info o jednotlivich stvorcekoch
        QVector< QVector<Player*> > m_squareBoard;

        // uchovava dlzku resp. vysku plochy v stvorcekoch
        int m_squareCount;

        // uchovava time limit pre tah
        int m_timeLimit;
};

#endif // GAME_H
