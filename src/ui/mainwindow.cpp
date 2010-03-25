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
    m_game = new Game(this);

    // vytvorenie plochy
    m_playBoard = new PlayBoard(this, m_game);
    setCentralWidget(m_playBoard);

    // pridanie hracov a spustenie hry
    QVector<Player*> plVec;
    plVec.push_back(new Player(NULL, new CirclePlayerSign(Qt::red)));
    plVec.push_back(new Player(NULL, new CrossPlayerSign(Qt::blue)));
    m_game->startGame(plVec);
}

MainWindow::~MainWindow() {

}


