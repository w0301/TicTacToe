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

#include "ui/playersign.h"
#include "ui/playercreator.h"
#include "ui/newgamedlg.h"

#include <QtGui>

// NewPlayerWidget class
NewPlayerWidget::NewPlayerWidget(QWidget *parent) :
        QWidget(parent), m_playerCreators(NULL), m_actualCreator(NULL), m_mainLayout(NULL) {
    // pridanie dat do combo boxu a jeho vytvorenie
    PlayerCreatorRegistrator::PlayerCreatorsList& list = PlayerCreatorRegistrator::list();
    m_playerCreators = new QComboBox(this);
    for(PlayerCreatorRegistrator::PlayerCreatorsList::const_iterator i = list.begin(); i != list.end(); i++) {
        m_playerCreators->addItem(i->first);
    }
    // nastavenie aktualenho indexu a postaranie sa o to aby sa pri zmene signalu zmenil
    // aj vytvarac
    m_playerCreators->setCurrentIndex(0);
    connect(m_playerCreators, SIGNAL(activated(int)), this, SLOT(setActualCreator(int)));

    // vytvorenie layoutu
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_playerCreators);

    // pridanie vytvaraca z indexu 0
    setActualCreator(0);
}

// nastavenie vytvaraca
void NewPlayerWidget::setActualCreator(int i) {
    if(i == -1) {
        return;
    }
    if(m_actualCreator != NULL) {
        delete m_actualCreator;
    }
    m_actualCreator = (PlayerCreatorRegistrator::list())[i].second();
    m_actualCreator->setParent(this);

    // pridanie do layoutu
    m_mainLayout->addWidget(m_actualCreator);
}

// NewGameDialog class
NewGameDialog::NewGameDialog(QWidget *parent) :
        QWizard(parent), m_playersCount(NULL), m_signsCountToWin(NULL),
        m_boardSize(NULL), m_timeLimit(NULL), m_playersPage(NULL) {
    addInitialPage();
    addPlayersPage();

    // po zmenenini na stranu hracov sa naplni
    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(fillPlayersPage(int)));

    // po prijati dialogu vytvorime novu hru
    connect(this, SIGNAL(accepted()), this, SLOT(createNewGame()));
}

NewGameDialog::~NewGameDialog() {

}

bool NewGameDialog::validateCurrentPage() {
    QWizard::validateCurrentPage();

    if(currentPage() == m_playersPage && PlayerSignRegistrator::hasOverloadSign()) {
        QMessageBox errBox(tr("Signs conflict!"), tr("It's not possible to have same sign\nfor more players."),
                           QMessageBox::Critical, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton, this);
        errBox.exec();
        return false;
    }
    return true;
}

// vytvorenie novej hry
void NewGameDialog::createNewGame() {
    // naplni zoznam hracov
    QVector<Player*> players;
    players.reserve(m_playersCount->text().toInt());
    for(QVector<NewPlayerWidget*>::const_iterator i = m_newPlayerWidgets.begin(); i != m_newPlayerWidgets.end(); i++) {
        players.push_back( (*i)->creator()->createPlayer() );
    }

    emit newGameCreated(
       new Game(m_boardSize->value(), m_timeLimit->text().toInt()*1000, m_signsCountToWin->value(), players, NULL)
   );
}

// vytvaranie stran
void NewGameDialog::addInitialPage() {
    QWizardPage *newPage = new QWizardPage;
    newPage->setTitle( tr("Main settings") );

    // pocet hracov
    QHBoxLayout *playersCountLayout = new QHBoxLayout;
    playersCountLayout->addWidget( new QLabel( tr("Players count: ") ) );
    playersCountLayout->addWidget( (m_playersCount = new QSpinBox) );
    m_playersCount->setValue(DEFAULT_PLAYERS_COUNT);
    // maximum bude pocet roznych podpisov
    m_playersCount->setRange(MINIMUM_PLAYERS_COUNT, PlayerSignRegistrator::list().size());

    // velkost plochy
    QHBoxLayout *boardSizeLayout = new QHBoxLayout;
    boardSizeLayout->addWidget( new QLabel( tr("Number of squares in board side: ") ) );
    boardSizeLayout->addWidget( (m_boardSize = new QSpinBox) );
    m_boardSize->setValue(DEFAULT_BOARD_SIZE);
    m_boardSize->setRange(MINIMUM_BOARD_SIZE, MAXIMUM_BOARD_SIZE);

    // pocet podpisov v rade na vytazstvo
    QHBoxLayout *signsCountToWinLayout = new QHBoxLayout;
    signsCountToWinLayout->addWidget( new QLabel( tr("Number of signs to win: ") ) );
    signsCountToWinLayout->addWidget( (m_signsCountToWin = new QSpinBox) );
    m_signsCountToWin->setValue(DEFAULT_WIN_STONES);
    m_signsCountToWin->setRange(MINIMUM_WIN_SIGNS_COUNT, MAXIMUM_BOARD_SIZE);

    // casovy limit tahu
    QHBoxLayout *timeLimitLayout = new QHBoxLayout;
    timeLimitLayout->addWidget( new QLabel( tr("Time limit of player's turn in seconds (0 = unlimited): ") ) );
    timeLimitLayout->addWidget( (m_timeLimit = new QLineEdit( QString().setNum(DEFAULT_TIME_LIMIT) )) );

    // pridanie sub layoutoch do hlavneho layoutu
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(playersCountLayout);
    mainLayout->addLayout(signsCountToWinLayout);
    mainLayout->addLayout(boardSizeLayout);
    mainLayout->addLayout(timeLimitLayout);

    // nastavenie layoutu a navrat funkcie
    newPage->setLayout(mainLayout);
    addPage(newPage);
}

void NewGameDialog::addPlayersPage() {
    // vitvorenie novej strany
    m_playersPage = new QWizardPage;
    m_playersPage->setTitle( tr("Players") );

    // vytvorenie layoutu
    QVBoxLayout *mainLayout = new QVBoxLayout(m_playersPage);

    // nastavenie layoutu a navrat funkcie
    m_playersPage->setLayout(mainLayout);
    addPage(m_playersPage);
}

void NewGameDialog::fillPlayersPage(int i) {
    if(page(i) != m_playersPage) {
        return;
    }
    // naplnenie strany pozadovanym poctom widgetov
    int size = m_playersCount->value();
    int oldSize = m_newPlayerWidgets.size();
    if(size < oldSize) {
        for(int i = size - 1; i != m_newPlayerWidgets.size(); i++) {
            delete m_newPlayerWidgets[i];
        }
    }
    m_newPlayerWidgets.resize(size);
    for(int i = oldSize; i != size; i++) {
        // vytvorime novy widget a pripojime ho tak aby sme vedeli ci
        // su vsetky konflikty podpisov vyriesene
        NewPlayerWidget *newWidget = new NewPlayerWidget;

        m_playersPage->layout()->addWidget(newWidget);
        m_newPlayerWidgets[i] = newWidget;
    }
}
