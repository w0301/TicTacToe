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

#include <QList>
#include <QTime>
#include <QPoint>
#include <QObject>
#include <QVector>

class Player;
class PlayBoard;
class MainWindow;

class Game : public QObject {
    Q_OBJECT

    public:
        class Square {
            public:
                Square() : m_player(NULL), m_isWinning(false) { };
                Square(Player *pl) : m_player(pl), m_isWinning(false) { };
                Square(const Square& from) : m_player(from.m_player), m_isWinning(false) { };

                Square& operator= (Player *pl) {
                    m_player = pl;
                    return *this;
                };
                Square& operator= (const Square& from) {
                    if(this != &from) {
                        m_player = from.m_player;
                        m_isWinning = from.m_isWinning;
                    }
                    return *this;
                };

                // implicitna konverzia na typ Player
                operator Player* () {
                    return m_player;
                };
                operator const Player* () const {
                    return m_player;
                };

                Player *operator-> () {
                    return m_player;
                };
                const Player *operator-> () const {
                    return m_player;
                };

                // zisti ci je stvorec vyherny a pod.
                bool isWinning() const {
                    return m_isWinning;
                };
                void setWinning(bool v = true) {
                    m_isWinning = v;
                };

            private:
                Player *m_player;
                bool m_isWinning;
        };

        // konstrutkory
        Game(int = DEFAULT_BOARD_SIZE, int = DEFAULT_TIME_LIMIT, int = DEFAULT_WIN_STONES,
             const QVector<Player*>& = QVector<Player*>(), QObject* = NULL);

        ~Game();

        // zistenie a nastavenie hraca, ktory je na rade
        Player *incActualPlayer();
        Player *actualPlayer() const;
        void setActualPlayerIndex(int i) {
            m_actualPlayerIndex = i;
        };

        // vrati a nastavi vector s hracmi
        QVector<Player*>& players() {
            return m_players;
        };
        const QVector<Player*>& players() const {
            return m_players;
        };
        void setPlayers(const QVector<Player*>&);
        void unsetPlayers();

        // vrati vektor plochy a vrati a nastavi jeho velkost
        QVector< QVector<Square> >& squareBoard() {
            return m_squareBoard;
        };
        const QVector< QVector<Square> >& squareBoard() const {
            return m_squareBoard;
        };
        Square& square(int x, int y) {
            return m_squareBoard[y][x];
        };
        const Square& square(int x, int y) const {
            return m_squareBoard[y][x];
        };
        int squareBoardSize() const {
            return m_squareCount;
        };
        void setSquareBoardSize(int);

        // vrati a nastavi pocet kamenov na vytazstvo
        int winStonesCount() const {
            return m_toWin;
        };
        void setWinStonesCount(int c) {
            m_toWin = c;
        };

        // je hra spustena?
        bool isRunning() const {
            return m_isRunning;
        };

        // je hra pozastavena?
        bool isPaused() const {
            return m_isPaused;
        };

        // event handlery
        void timerEvent(QTimerEvent*);

    public slots:
        void fillSquare(int, int, Player*);
        void testWinner(int, int, Player*);
        void processActualPlayer(int = -1, int = -1);

        // spustanie hry a pod
        void startGame(const QVector<Player*>&);
        void startGame();
        void pauseGame(bool = true);
        void unpauseGame() {
            pauseGame(false);
        };
        void stopGame();
        void resetGame();

        // sprava timeru
        void resetTimer(int = -1);
        void stopTimer();

    signals:
        // posle signal o tom, ze doslo k zmene na hracej ploche
        // ako parametre posle index zmeneneho stvorceka
        // [WHOLE, WHOLE] signalizuje zmenu celej plochy, parameter Player
        // je pouzity iba pri odosielani objektom hracov
        void squareBoardUpdated(int = WHOLE, int = WHOLE, Player* = NULL);

        // signal je zavolany, ked nastane sutuacia zmenenia hraca
        // ako parameter posiele ukazatel na noveho hraca
        void playerChanged(Player*);

        // signal je poslany ked je hra zacata
        // ako parameter posiela prveho hraca, kt. je aj na tahu
        void gameStarted(Player* = NULL);

        // signal je poslany, ked hra skonci, ukazatel
        // je poslany v pripade, ze je ukoncene v pripade vyhri hraca
        void gameEnded(Player* = NULL);

        // posle sa ked bola hra pauznuta/odpauznuta
        void gamePaused(bool = true);
        void gameUnpaused();

        // signali su poslane na zaciatku/konci slotu processPlayer()
        // posielanie zabezpecuje to aby nedochadzalo k spracovaniu kliknuti
        // ktore nemaju vyznam lebo je na rade iny, halvne sietovy hrac
        void playerProcessStarted();
        void playerProcessEnded();

        // signal je poslany, ked je znizeny time limit
        // a posiela novy time limit
        void timerUpdated(int);

    private:
        // uchovava vsetkych hracov hry
        QVector<Player*> m_players;

        // urcuje, ktory hrac je na rade
        int m_actualPlayerIndex;

        // uchovava info o jednotlivich stvorcekoch
        QVector< QVector<Square> > m_squareBoard;

        // je hra spustena?
        bool m_isRunning;

        // je hra pauznuta
        bool m_isPaused;

        // uchovava dlzku resp. vysku plochy v stvorcekoch
        int m_squareCount;

        // uchovava time limit pre tah - v milisekundach
        int m_timeLimit;

        // uchovava aktualny cas, kt. zostava na tah
        int m_actuTimeLimit;

        // ID timera, kt. pocita time limit
        int m_timerID;

        // cas, ktory bol pri zacati kola
        QTime m_lastTime;

        // uchovava cislo, kt. urcuje kolko kamenov, podpisov musi
        // byt v rade na vytazstvo
        int m_toWin;
};

#endif // GAME_H
