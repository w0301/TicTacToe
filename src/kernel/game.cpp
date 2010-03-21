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

Game::Game(QVector<Player*> players, MainWindow *parent, int size, int time) :
        QObject(parent), m_players(players), m_actualPlayerIndex(0), m_playBoard( new PlayBoard(parent, size) ),
        m_timeLimit(time) {
    // rodic dostane centralny widget
    parent->setCentralWidget(m_playBoard);

    // pripojima signal, ktory je poslany pri kliknuti na plochu
    connect(m_playBoard, SIGNAL(squareClicked(int,int)), this, SLOT(processPlayer(int, int)));

    // pripojime signal kazdeho hraca k slotu plochy a nastavime rodica
    foreach(Player *i, m_players) {
        i->setParent(this);
        connect(i, SIGNAL(moving(int,int)), m_playBoard, SLOT(putPlayer(int, int)));
    }
}

void Game::incActualPlayer() {
    if(m_actualPlayerIndex+1 < m_players.size()) {
        m_actualPlayerIndex++;
    }
    else {
        m_actualPlayerIndex = 0;
    }
}

Player *Game::actualPlayer() const {
    if(m_actualPlayerIndex >= m_players.size()) {
        return NULL;
    }
    return m_players[m_actualPlayerIndex];
};

// slot je zavolany po kliknuti na hraciu plochu
// ako parametre berie suradnice kliknuteho stvorceka v poli
void Game::processPlayer(int arrX, int arrY) {
    Player *actPl = actualPlayer();
    if(actPl != NULL && !actPl->isComputer()) {
        // rucne zavolame slot, ktory zabezpeci pohyb
        // no len ked mozeme vlozit krizik
        if(m_playBoard->playerOn(arrX, arrY) == NULL) {
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
