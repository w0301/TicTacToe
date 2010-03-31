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
    setToolTip(tr("Time limit of move"));
}

void TimeLimitFrame::showTimeLimit(int num) {
    display( double(num) / 1000.0 );
}

void TimeLimitFrame::resetTimeLimit() {
    display(0);
}

// PlayerSignFrame class
PlayerSignFrame::PlayerSignFrame(QWidget *parent) :
        QFrame(parent), m_actuPlayer(NULL) {
    setMinimumSize(16, 16);
    setMaximumSize(16, 16);
    setToolTip(tr("Sign of player"));
}

// nastavi hraca, ktoreho podpis sa bude vykreslovat
void PlayerSignFrame::setPlayer(Player *pl) {
    m_actuPlayer = pl;
    repaint();
}

// vykresli podpis hraca
void PlayerSignFrame::paintEvent(QPaintEvent*) {
    if(player() == NULL) {
        return;
    }
    QPainter painter(this);
    painter.drawPixmap(0, 0, player()->sign()->signPixmap(this, QPoint(0, 0), 16));
}

// PlayerListFrame class
PlayerListFrame::PlayerListFrame(QWidget *parent) :
        QFrame(parent), m_game(NULL), m_actuPlayerName(NULL), m_playerList(NULL) {
    // pridanie widgetu s menom
    m_actuPlayerName = new QLabel(this);

    // pridanie widgetu s podpisom
    m_actuPlayerSign = new PlayerSignFrame(this);

    // pridanie widgetu listu hracov
    m_playerList = new QListWidget(this);
    m_playerList->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

    // prve dve do horizontalneho layoutu
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(m_actuPlayerName);
    hLayout->addWidget(m_actuPlayerSign);

    // a potom do vertikalneho layoutu
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(m_playerList);
}

// naplni list menami hracov
void PlayerListFrame::fillList() {
    if(m_game == NULL) {
        return;
    }
    foreach(Player *i, m_game->players()) {
        m_playerList->addItem(i->name());
    }
}

// nastavi label aktualneho hraca
void PlayerListFrame::setActualPlayer(Player *pl) {
    if(m_actuPlayerName != NULL) {
        m_actuPlayerName->setText(pl->name());
        m_actuPlayerName->setToolTip(tr("Actually moving"));
    }
    if(m_actuPlayerSign != NULL) {
        m_actuPlayerSign->setPlayer(pl);
    }
}

// nastavi hru s ktorov bude widget pracovat
void PlayerListFrame::setGame(Game *game) {
    unsetGame();
    m_game = game;
    if(m_game != NULL) {
        // pri zacati hry sa naplni list
        connect(m_game, SIGNAL(gameStarted(Player*)), this, SLOT(fillList()));

        // ked sa zmeni hrac nastavime label
        connect(m_game, SIGNAL(playerChanged(Player*)), this, SLOT(setActualPlayer(Player*)));

        // ked sa hra skonci musime vicistit list a label
        connect(m_game, SIGNAL(gameStopped()), m_playerList, SLOT(clear()));
        connect(m_game, SIGNAL(gameStopped()), m_actuPlayerName, SLOT(clear()));
    }
}

void PlayerListFrame::unsetGame() {
    if(m_game != NULL) {
        disconnect(m_game, SIGNAL(gameStarted(Player*)), this, SLOT(fillList()));
        disconnect(m_game, SIGNAL(playerChanged(Player*)), this, SLOT(setActualPlayer(Player*)));
        disconnect(m_game, SIGNAL(gameStopped()), m_playerList, SLOT(clear()));
        disconnect(m_game, SIGNAL(gameStopped()), m_actuPlayerName, SLOT(clear()));

        m_game = NULL;
    }
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
    m_timeLimitFrame = new TimeLimitFrame(leftDockWidgets, 0);

    // vytvorenie listu hracov
    m_playerListFrame = new PlayerListFrame(leftDockWidgets);

    // vytvorenie check boxu na pauzu
    m_pauseCheckBox = new QCheckBox(tr("&Pause"), leftDockWidgets);

    // pridanie do layoutu
    leftDockLayout->addWidget(m_timeLimitFrame);
    leftDockLayout->addWidget(m_playerListFrame);
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
    plVec.push_back(new Player(NULL, new CirclePlayerSign(Qt::red), "Wizard"));
    plVec.push_back(new Player(NULL, new CrossPlayerSign(Qt::blue), "Arcan"));
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
        m_game->setParent(this);

        /// v prvom rade nastavenie hry hracej plochy
        m_playBoard->setGame(m_game);

        /// potom nastavenie hry zobrazovaca hracov
        m_playerListFrame->setGame(m_game);

        /// dalej pripojenie signalov
        // signal pre update casoveho limitu
        connect(m_game, SIGNAL(gameStarted(Player*)), m_timeLimitFrame, SLOT(resetTimeLimit()));
        connect(m_game, SIGNAL(gameStopped()), m_timeLimitFrame, SLOT(resetTimeLimit()));
        connect(m_game, SIGNAL(timerUpdated(int)), m_timeLimitFrame, SLOT(showTimeLimit(int)));

        // signali pre check box pauzi na docku
        connect(m_pauseCheckBox, SIGNAL(clicked(bool)), m_game, SLOT(pauseGame(bool)));
        connect(m_game, SIGNAL(gamePaused(bool)), m_pauseCheckBox, SLOT(setChecked(bool)));
    }
}

// zrusi aktulnu hru v okne
void MainWindow::unsetGame() {
    if(m_game != NULL) {
        /// odstranenie hry dalsich objektov
        m_playBoard->unsetGame();
        m_playerListFrame->unsetGame();

        // odpojenie signalov
        disconnect(m_game, SIGNAL(gameStarted(Player*)), m_timeLimitFrame, SLOT(resetTimeLimit(int)));
        disconnect(m_game, SIGNAL(gameStopped()), m_timeLimitFrame, SLOT(resetTimeLimit(int)));
        disconnect(m_game, SIGNAL(timerUpdated(int)), m_timeLimitFrame, SLOT(showTimeLimit(int)));
        disconnect(m_pauseCheckBox, SIGNAL(clicked(bool)), m_game, SLOT(pauseGame(bool)));
        disconnect(m_game, SIGNAL(gamePaused()), m_pauseCheckBox, SLOT(click()));

        // a nakoniec zmazanie samotnej hry
        delete m_game;
        m_game = NULL;
    }
}

