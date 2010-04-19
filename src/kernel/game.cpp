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

#include "kernel/game.h"
#include "kernel/player.h"
#include "ui/playboard.h"
#include "ui/mainwindow.h"

#include <Qt>

// vytvori objekt a nastavi vector s hracmi
Game::Game(int size, int time, int toWin, const QVector<Player*>& players, QObject *parent) :
        QObject(parent), m_players(), m_actualPlayerIndex(0), m_squareBoard(),
        m_isRunning(false), m_isPaused(false), m_squareCount(size), m_timeLimit(time),
        m_actuTimeLimit(time), m_timerID(-1), m_toWin(toWin) {
    setSquareBoardSize(m_squareCount);
    setPlayers(players);
}

Game::~Game() {

}

// nastavi novu velkost plochy hry - plocha je
// zvacsena/zmensena a prvky su zachovane
void Game::setSquareBoardSize(int size) {
    m_squareBoard.resize(size);
    for(int i = 0; i != size; i++) {
        m_squareBoard[i].resize(size);
    }
}

// zamaze hracov
void Game::unsetPlayers() {
    foreach(Player *i, players()) {
        delete i;
    }
    m_players.clear();
}

// nastaci novy vektor s hracmi - indikuje zaciatok hry
void Game::setPlayers(const QVector<Player*>& players) {
    if(players.size() < 2) {
        return;
    }
    unsetPlayers();
    m_players = players;

    // pripojime signal kazdeho hraca k slotu plochy a nastavime rodica
    foreach(Player *i, m_players) {
        i->setParent(this);

        // reakcia na tah hraca - naplnime v nej pole
        connect(i, SIGNAL(moving(int, int, Player*)), this, SLOT(fillSquare(int, int, Player*)));

        // po kazdom tahu zistime ci nebol tah vytazny
        connect(i, SIGNAL(moving(int, int, Player*)), this, SLOT(testWinner(int, int, Player*)));

        // musime dat vediet hracovi ze sa zmenila plocha - mozno to bude chciet poslat cez siet domov
        connect(this, SIGNAL(squareBoardUpdated(int, int, Player*)), i, SLOT(updateBoard(int, int, Player*)));
    }
}

// prepne hraca, ktory je na rade na dalsieho a vrati
// jeho odkaz
Player *Game::incActualPlayer() {
    if(m_actualPlayerIndex+1 < m_players.size()) {
        m_actualPlayerIndex++;
    }
    else {
        m_actualPlayerIndex = 0;
    }

    emit playerChanged(actualPlayer());
    return actualPlayer();
}

// vrati hraca, ktory je prave na rade
Player *Game::actualPlayer() const {
    if(m_actualPlayerIndex >= m_players.size()) {
        return NULL;
    }
    return m_players[m_actualPlayerIndex];
};

// slot zapise na poziciu [x, y] hraca pl, samozrejme
// pokial tam nieje uz iny hrac
void Game::fillSquare(int x, int y, Player *pl) {
    if(m_squareBoard[y][x] == NULL) {
        m_squareBoard[y][x] = pl;

        // dame vediet ze doslo k zmene - widgetu aj hracom
        // a kto ju vykonal - potrebne iba pre sietovych hracov (plocha pouziva pole hry)
        emit squareBoardUpdated(x, y, pl);
    }
}

// slot otestuje ci predany hrac pl nevyhral tahom na [x, y]
void Game::testWinner(int x, int y, Player *pl) {
    // uchovava kolko je stvorcekov za sebou
    int goodSquares = 1, nx = 0, ny = 0;
    bool dir1 = true, dir2 = true;
    QPoint startP, endP;
    startP = endP = QPoint(x, y);

    // smer vodorovne
    if(goodSquares != winStonesCount()) {
        for(int i = 1; goodSquares != winStonesCount(); i++) {
            nx = x - i;
            if(dir1 && nx >= 0 && square(nx, y) == pl) {
                goodSquares++;
                startP = QPoint(nx, y);
            }
            else {
                dir1 = false;
            }

            nx = x + i;
            if(dir2 && nx < squareBoardSize() && square(nx, y) == pl) {
                goodSquares++;
                endP = QPoint(nx, y);
            }
            else {
                dir2 = false;
            }
            if(!dir1 && !dir2) {
                break;
            }
        }
    }

    // smer vertikalne
    if(goodSquares != winStonesCount()) {
        goodSquares = 1;
        dir1 = dir2 = true;
        startP = endP = QPoint(x, y);
        for(int i = 1; goodSquares != winStonesCount(); i++) {
            ny = y - i;
            if(dir1 && ny >= 0 && square(x, ny) == pl) {
                goodSquares++;
                startP = QPoint(x, ny);
            }
            else {
                dir1 = false;
            }

            ny = y + i;
            if(dir2 && ny < squareBoardSize() && square(x, ny) == pl) {
                goodSquares++;
                endP = QPoint(x, ny);
            }
            else {
                dir2 = false;
            }
            if(!dir1 && !dir2) {
                break;
            }
        }
    }

    // smer sikmo - zlava doprava
    if(goodSquares != winStonesCount()) {
        goodSquares = 1;
        dir1 = dir2 = true;
        startP = endP = QPoint(x, y);
        for(int i = 1; goodSquares != winStonesCount(); i++) {
            ny = y - i;
            nx = x - i;
            if(dir1 && ny >= 0 && nx >= 0 && square(nx, ny) == pl) {
                goodSquares++;
                startP = QPoint(nx, ny);
            }
            else {
                dir1 = false;
            }

            ny = y + i;
            nx = x + i;
            if(dir2 && ny < squareBoardSize() && nx < squareBoardSize() && square(nx, ny) == pl) {
                goodSquares++;
                endP = QPoint(nx, ny);
            }
            else {
                dir2 = false;
            }
            if(!dir1 && !dir2) {
                break;
            }
        }
    }

    // smer sikmo - zprava dolava
    if(goodSquares != winStonesCount()) {
        goodSquares = 1;
        dir1 = dir2 = true;
        startP = endP = QPoint(x, y);
        for(int i = 1; goodSquares != winStonesCount(); i++) {
            ny = y - i;
            nx = x + i;
            if(dir1 && ny >= 0 && nx < squareBoardSize() && square(nx, ny) == pl) {
                goodSquares++;
                startP = QPoint(nx, ny);
            }
            else {
                dir1 = false;
            }

            ny = y + i;
            nx = x - i;
            if(dir2 && ny < squareBoardSize() && nx >= 0 && square(nx, ny) == pl) {
                goodSquares++;
                endP = QPoint(nx, ny);
            }
            else {
                dir2 = false;
            }
            if(!dir1 && !dir2) {
                break;
            }
        }
    }

    // ked sme nasli vytaza posleme signal
    if(goodSquares == winStonesCount()) {
        if(startP.x() == endP.x()) {
            // vertykalny smer
            for(int i = 0; i != winStonesCount(); i++) {
                square(startP.x(), startP.y()+i).setWinning(true);
            }
        }
        else if(startP.y() == endP.y()) {
            // vodorovny smer
            for(int i = 0; i != winStonesCount(); i++) {
                square(startP.x()+i, startP.y()).setWinning(true);
            }
        }
        else if(startP.x() < endP.x()) {
            // z praveho horneho do laveho dolneho
            for(int i = 0; i != winStonesCount(); i++) {
                square(startP.x()+i, startP.y()+i).setWinning(true);
            }
        }
        else if(startP.x() > endP.x()) {
            // z laveho horneho do praveho dolneho
            for(int i = 0; i != winStonesCount(); i++) {
                square(startP.x()-i, startP.y()+i).setWinning(true);
            }
        }
        emit squareBoardUpdated();
        emit gameEnded(pl);
    }
}

// slot je zavolany po kliknuti na hraciu plochu
// ako parametre berie suradnice kliknuteho stvorceka v poli
void Game::processActualPlayer(int arrX, int arrY) {
    emit playerProcessStarted();

    Player *actPl = actualPlayer();
    if(actPl != NULL && !actPl->isComputer()) {
        // rucne zavolame slot, ktory zabezpeci pohyb
        // no len ked mozeme vlozit krizik
        if(square(arrX, arrY) == NULL) {
            actPl->processMove(arrX, arrY);
            actPl = incActualPlayer();
        }
        else {
            emit playerProcessEnded();
            return;
        }
    }

    // postarame sa o to aby sa pripdae NPCka alebo sietovy hraci
    // dostali k tahu
    while(actPl != NULL && !actPl->isOrdinary()) {
        actPl->processMove();
        actPl = incActualPlayer();
    }
    emit playerProcessEnded();
}

// resetne timer na time limit hry
void Game::resetTimer(int l) {
    if(m_timeLimit != 0) {
        if(l == -1) {
            m_actuTimeLimit = m_timeLimit;
        }
        else {
            m_actuTimeLimit = l;
        }
        timerUpdated(m_actuTimeLimit);
        m_lastTime.start();
        if(m_timerID == -1) {
            m_timerID = startTimer(500);
            connect(this, SIGNAL(gameEnded(Player*)), this, SLOT(stopTimer()));
            connect(this, SIGNAL(playerChanged(Player*)), this, SLOT(resetTimer()));
        }
    }
}

// zastavi timer
void Game::stopTimer() {
    if(m_timerID != -1) {
        killTimer(m_timerID);
        disconnect(this, SIGNAL(gameEnded(Player*)), this, SLOT(stopTimer()));
        disconnect(this, SIGNAL(playerChanged(Player*)), this, SLOT(resetTimer()));
        m_timerID = -1;
    }
}

// spusti hru, ktora uz ma hracov
void Game::startGame() {
    if(isRunning()) {
        return;
    }
    setActualPlayerIndex(0);
    m_isRunning = true;
    resetTimer();

    emit gameStarted(actualPlayer());
    emit playerChanged(actualPlayer());
}

// spusti hru s predanymi hracmi
void Game::startGame(const QVector<Player*>& players) {
    if(isRunning() || players.size() < 2) {
        return;
    }
    setPlayers(players);
    startGame();
}

// pozastavi hru
void Game::pauseGame(bool val) {
    m_isPaused = val;
    if(val) {
        emit playerProcessStarted();
        stopTimer();
        emit gamePaused(true);
    }
    else {
        // resetneme a nastavime uz uplinuli cas tahu
        resetTimer( m_actuTimeLimit );
        emit gameUnpaused();
        emit gamePaused(false);
        emit playerProcessEnded();
    }
}

// zastavi hru - vycisti plochu, vycisti pole hracov
// a nastavi premennu m_isRunning
void Game::stopGame() {
    m_squareBoard.clear();
    setSquareBoardSize(squareBoardSize());
    unsetPlayers();
    m_isRunning = false;

    stopTimer();

    // posleme signaly
    emit gameEnded();
    emit squareBoardUpdated(CLEAR, CLEAR);
}

// vycisti pole kamenov a posle signal o update hry
// no signal o zacati neposiela - reset != start!
void Game::resetGame() {
    if(!isRunning()) {
        return;
    }
    m_squareBoard.clear();
    setSquareBoardSize(squareBoardSize());
    setActualPlayerIndex(0);
    m_isRunning = true;
    resetTimer();

    emit gameStarted(actualPlayer());
    emit playerChanged(actualPlayer());
}

// event handler timeru
void Game::timerEvent(QTimerEvent*) {
    if(!isRunning() || isPaused()) {
        return;
    }
    if(m_actuTimeLimit <= 0) {
        incActualPlayer();
    }
    else if(m_lastTime.elapsed() >= 1000) {
        m_actuTimeLimit -= 1000;
        timerUpdated(m_actuTimeLimit);
        m_lastTime.restart();
    }
}
