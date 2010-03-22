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

#include "main.h"
#include "kernel/game.h"
#include "kernel/player.h"
#include "ui/mainwindow.h"
#include "ui/playboard.h"

#include <QtGui>

MainWindow::MainWindow() : QMainWindow(NULL) {
    // nastavenie defaultnych hodnot
    setWindowTitle( tr("TicTacToe") );
    setMinimumSize(405, 405);

    // vytvorenie novej hry a priradenie centralneho
    // widgetu v konstruktore
    m_game = new Game(this, DEFAULT_BOARD_SIZE, DEFAULT_TIME_LIMIT);

    // pridanie hracov
    QVector<Player*> plVec;
    plVec.push_back(new Player(NULL, Player::Circle, Qt::red));
    plVec.push_back(new Player(NULL, Player::Cross, Qt::blue));
    m_game->setPlayers(plVec);

    // vytvorenie plochy
    m_playBoard = new PlayBoard(this, m_game);
    setCentralWidget(m_playBoard);
}

MainWindow::~MainWindow() {

}


