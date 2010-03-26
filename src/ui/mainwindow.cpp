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

// TimeLimitFrame class
TimeLimitFrame::TimeLimitFrame(QWidget *parent, int num) : QLCDNumber(parent) {
    display(num);
}

void TimeLimitFrame::showTimeLimit(int num) {
    display( double(num) / 1000.0 );
}

// MainWindow class
MainWindow::MainWindow() :
        QMainWindow(NULL), m_game(NULL), m_playBoard(NULL), m_timeLimitFrame(NULL),
        m_leftDock(NULL) {
    /// nastavenie defaultnych hodnot
    setWindowTitle( tr("TicTacToe") );
    setMinimumSize(505, 405);

    /// vytvorenie hracej plochy
    m_playBoard = new PlayBoard(this);
    setCentralWidget(m_playBoard);

    /// vytvorenie laveho docku
    m_leftDock = new QDockWidget(this);
    m_leftDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_leftDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    // pomocny widget a layout
    QWidget *leftDockWidgets = new QWidget(m_leftDock);
    QVBoxLayout *leftDockLayout = new QVBoxLayout(leftDockWidgets);

    // vytvorenie zobrazovaca casu
    m_timeLimitFrame = new TimeLimitFrame(this, 0);

    // vytvorenie check boxu na pauzu
    m_pauseCheckBox = new QCheckBox(tr("&Pause"), leftDockWidgets);

    // pridanie do layoutu
    leftDockLayout->addWidget(m_timeLimitFrame);
    leftDockLayout->addStretch(1);
    leftDockLayout->addWidget(m_pauseCheckBox);

    // nastavenie widgetu dockovacieho widgetu
    m_leftDock->setWidget(leftDockWidgets);

    // priradenie docku oknu
    addDockWidget(Qt::LeftDockWidgetArea, m_leftDock);


    // vytvorenie novej hry a priradenie centralneho
    // widgetu v konstruktore
    setGame( new Game(this, DEFAULT_BOARD_SIZE, 30000, DEFAULT_WIN_STONES) );

    // pridanie hracov a spustenie hry
    QVector<Player*> plVec;
    plVec.push_back(new Player(NULL, new CirclePlayerSign(Qt::red)));
    plVec.push_back(new Player(NULL, new CrossPlayerSign(Qt::blue)));
    m_game->startGame(plVec);
}

MainWindow::~MainWindow() {

}

// nastavi hru okna a pripoji k oknu potrebne signaly a zobrazi
// potrebne okna
void MainWindow::setGame(Game *game) {
    unsetGame();
    m_game = game;
    if(m_game != NULL) {
        /// v prvom rade nastavenie hry hracej plochy
        m_playBoard->setGame(m_game);

        /// dalej pripojenie signalov
        // signal pre update casoveho limitu
        connect(m_game, SIGNAL(timerUpdated(int)), m_timeLimitFrame, SLOT(showTimeLimit(int)));

        // signali pre check box pauzi na docku
        connect(m_pauseCheckBox, SIGNAL(clicked(bool)), m_game, SLOT(pauseGame(bool)));
        connect(m_game, SIGNAL(gamePaused(bool)), m_pauseCheckBox, SLOT(setChecked(bool)));
    }
}

// zrusi aktulnu hru v okne
void MainWindow::unsetGame() {
    if(m_game != NULL) {
        /// odstavenie hry hracej plochy
        m_playBoard->unsetGame();

        // odpojenie signalov
        disconnect(m_game, SIGNAL(timerUpdated(int)), m_timeLimitFrame, SLOT(showTimeLimit(int)));
        disconnect(m_pauseCheckBox, SIGNAL(clicked(bool)), m_game, SLOT(pauseGame(bool)));
        disconnect(m_game, SIGNAL(gamePaused()), m_pauseCheckBox, SLOT(click()));

        // a nakoniec zmazanie samotnej hry
        delete m_game;
        m_game = NULL;
    }
}

