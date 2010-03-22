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

Game::Game(QVector<Player*> players, QObject *parent, int size, int time) :
        QObject(parent), m_players(), m_actualPlayerIndex(0),
        m_squareBoard(), m_squareCount(size), m_timeLimit(time) {
    setPlayers(players);
    setSquareBoardSize(m_squareCount);
}

Game::Game(QObject *parent, int size, int time) :
        QObject(parent), m_players(), m_actualPlayerIndex(0),
        m_squareBoard(), m_squareCount(size), m_timeLimit(time) {
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
        connect(i, SIGNAL(moving(int, int)), this, SLOT(fillSquare(int, int)));
    }
};

// prepne hraca, ktory je na rade na dalsieho
void Game::incActualPlayer() {
    if(m_actualPlayerIndex+1 < m_players.size()) {
        m_actualPlayerIndex++;
    }
    else {
        m_actualPlayerIndex = 0;
    }
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
void Game::fillSquare(int x, int y) {
    Player *pl = qobject_cast<Player*>(sender());
    if(m_squareBoard[y][x] == NULL) {
        m_squareBoard[y][x] = pl;
        emit squareBoardUpdated(x, y);
    }
}

// slot je zavolany po kliknuti na hraciu plochu
// ako parametre berie suradnice kliknuteho stvorceka v poli
void Game::processPlayer(int arrX, int arrY) {
    Player *actPl = actualPlayer();
    if(actPl != NULL && !actPl->isComputer()) {
        // rucne zavolame slot, ktory zabezpeci pohyb
        // no len ked mozeme vlozit krizik
        if(square(arrX, arrY) == NULL) {
            actPl->processMove(arrX, arrY);
            incActualPlayer();
            actPl = actualPlayer();
        }
        else {
            return;
        }
    }

    // postarame sa o to aby sa pripdae NPC dostalo k tahu
    if(actPl != NULL && actPl->isComputer()) {
        actPl->processMove();
    }
}
