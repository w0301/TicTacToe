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
    m_game = new Game(this, DEFAULT_BOARD_SIZE, 5000, DEFAULT_WIN_STONES);

    // vytvorenie plochy
    m_playBoard = new PlayBoard(this, m_game);
    setCentralWidget(m_playBoard);

    // pridanie hracov a spustenie hry
    QVector<Player*> plVec;
    plVec.push_back(new Player(NULL, new CirclePlayerSign(Qt::red)));
    plVec.push_back(new Player(NULL, new CrossPlayerSign(Qt::blue)));

    QDockWidget *dockWidget = new QDockWidget(tr("Dock Widget"), this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget *dock = new QWidget(dockWidget);
    QGridLayout *layout = new QGridLayout(dock);

    edit = new QLineEdit(dock);
    layout->addWidget(edit, 0, 0);
    connect(m_game, SIGNAL(timerUpdated(int)), this, SLOT(updateTime(int)));

    QCheckBox *push = new QCheckBox(tr("Pause"), dock);
    layout->addWidget(push, 1, 0);
    connect(push, SIGNAL(clicked(bool)), m_game, SLOT(pauseGame(bool)));

    dockWidget->setWidget(dock);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

    QDockWidget *dockWidget2 = new QDockWidget(tr("Dock Widget2"), this);
    dockWidget2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget2);

    m_game->startGame(plVec);
}

MainWindow::~MainWindow() {

}

void MainWindow::updateTime(int val) {
    edit->setText( QString("%1").arg( float(val) / 1000.0 ) );
}

