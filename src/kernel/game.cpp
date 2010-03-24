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

Game::Game(QVector<Player*> players, QObject *parent, int size, int time, int toWin) :
        QObject(parent), m_players(), m_actualPlayerIndex(0),
        m_squareBoard(), m_squareCount(size), m_timeLimit(time), m_toWin(toWin) {
    setPlayers(players);
    setSquareBoardSize(m_squareCount);
}

Game::Game(QObject *parent, int size, int time, int toWin) :
        QObject(parent), m_players(), m_actualPlayerIndex(0),
        m_squareBoard(), m_squareCount(size), m_timeLimit(time), m_toWin(toWin) {
    setSquareBoardSize(m_squareCount);
}

void Game::setSquareBoardSize(int size) {
    m_squareBoard.resize(size);
    for(int i = 0; i != size; i++) {
        m_squareBoard[i].resize(size);
    }
}

// nastaci novy vektor s hracmi
void Game::setPlayers(QVector<Player*> players) {
    m_players = players;

    // pripojime signal kazdeho hraca k slotu plochy a nastavime rodica
    foreach(Player *i, m_players) {
        i->setParent(this);

        // reakcia na tah hraca - naplnime v nej pole
        connect(i, SIGNAL(moving(int, int, Player*)), this, SLOT(fillSquare(int, int, Player*)));

        // po kazdom tahu zistime ci nebol tah vytazny
        connect(i, SIGNAL(moving(int, int, Player*)), this, SLOT(testWinner(int,int,Player*)));

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
// pokial tam nieje uz iny hrac - moze byt volane iba cez signal
void Game::fillSquare(int x, int y, Player *pl) {
    if(m_squareBoard[y][x] == NULL) {
        m_squareBoard[y][x] = pl;

        // dame vediet ze doslo k zmene - widgetu aj hracom
        // a kto ju vykonal - potrebne iba pre sietovych hracov (plocha pouziva pole hry)
        emit squareBoardUpdated(x, y, pl);
    }
}

// slot otestuje ci predany hrac pl nevyhral tahom na [x, y]
void Game::testWinner(int x, int y, Player *pl) const {
    // uchovava kolko je stvorcekov za sebou
    int goodSquares = 1;
    bool dir1 = true, dir2 = true;

    // smer vodorovne
    for(int i = 1; goodSquares != winStonesCount(); i++) {
        if(dir1 && x - i >= 0 && square(x - i, y) == pl) {
            goodSquares++;
        }
        else {
            dir1 = false;
        }
        if(dir2 && x + i < squareBoardSize() && square(x + i, y) == pl) {
            goodSquares++;
        }
        else {
            dir2 = false;
        }
        if(!dir1 && !dir2) {
            break;
        }
    }
    if(goodSquares == winStonesCount()) {
        emit playerWon(pl);
        return;
    }

    // smer vertikalne
    goodSquares = 1;
    dir1 = dir2 = true;
    for(int i = 1; goodSquares != winStonesCount(); i++) {
        if(dir1 && y - i >= 0 && square(x, y - i) == pl) {
            goodSquares++;
        }
        else {
            dir1 = false;
        }
        if(dir2 && y + i < squareBoardSize() && square(x, y + i) == pl) {
            goodSquares++;
        }
        else {
            dir2 = false;
        }
        if(!dir1 && !dir2) {
            break;
        }
    }
    if(goodSquares == winStonesCount()) {
        emit playerWon(pl);
        return;
    }

    // smer sikmo - zlava doprava
    goodSquares = 1;
    dir1 = dir2 = true;
    for(int i = 1; goodSquares != winStonesCount(); i++) {
        if(dir1 && y - i >= 0 && x - i >= 0 && square(x - i, y - i) == pl) {
            goodSquares++;
        }
        else {
            dir1 = false;
        }
        if(dir2 && y + i < squareBoardSize() && x + i < squareBoardSize() && square(x + i, y + i) == pl) {
            goodSquares++;
        }
        else {
            dir2 = false;
        }
        if(!dir1 && !dir2) {
            break;
        }
    }
    if(goodSquares == winStonesCount()) {
        emit playerWon(pl);
        return;
    }

    // smer sikmo - zprava dolava
    goodSquares = 1;
    dir1 = dir2 = true;
    for(int i = 1; goodSquares != winStonesCount(); i++) {
        if(dir1 && y - i >= 0 && x + i < squareBoardSize() && square(x + i, y - i) == pl) {
            goodSquares++;
        }
        else {
            dir1 = false;
        }
        if(dir2 && y + i < squareBoardSize() && x - i >= 0 && square(x - i, y + i) == pl) {
            goodSquares++;
        }
        else {
            dir2 = false;
        }
        if(!dir1 && !dir2) {
            break;
        }
    }
    if(goodSquares == winStonesCount()) {
        emit playerWon(pl);
        return;
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

void Game::resetGame() {
    m_squareBoard.clear();
    setSquareBoardSize(squareBoardSize());
    setActualPlayerIndex(0);

    // dame vediet ze doslo k zmene celej plochy
    emit squareBoardUpdated();
}
