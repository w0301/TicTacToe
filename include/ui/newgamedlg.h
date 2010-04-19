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

#ifndef NEWGAMEDLG_H
#define NEWGAMEDLG_H

#include "main.h"

#include <QHash>
#include <QList>
#include <QVector>
#include <QWizard>

class Game;
class PlayerCreatorBase;

class QLabel;
class QSpinBox;
class QLineEdit;
class QComboBox;
class QWizardPage;
class QVBoxLayout;
class QScrollArea;

// NewPlayerWidget class
class NewPlayerWidget : public QWidget {
    Q_OBJECT

    public:
        NewPlayerWidget(QWidget* = NULL);
        ~NewPlayerWidget() { };

        // vrati vytvarac hraca
        PlayerCreatorBase *creator() {
            return m_actualCreator;
        };
        const PlayerCreatorBase *creator() const {
            return m_actualCreator;
        };

    public slots:
        // nastavi vytvarac podla indexu vo vectore NewGameDialog::creatorsList
        void setActualCreator(int);

    private:
        // uchovava zoznam vsetkych moznych vytvaracov
        QComboBox *m_playerCreators;

        // uchovava widget prave aktualneho vytvaraca
        PlayerCreatorBase *m_actualCreator;

        // layout, kt. usporiadava 2 widgety
        QVBoxLayout *m_mainLayout;
};

// NewGameDialog class
class NewGameDialog : public QWizard {
    Q_OBJECT

    public:
        // vytvori dialog
        NewGameDialog(QWidget* = NULL);

        // znici dialog
        ~NewGameDialog();

        // zisti ci sa mozeme posunut dalej
        bool validateCurrentPage();

    public slots:
        // vytvori hru a posle ju signalom newGameCreated
        void createNewGame();

        // vytvori a prida jednotlive strany
        void addInitialPage();
        void addPlayersPage();

        // naplni stranu s hracmi + testuje ci sa zmenilo na stranu hracov
        void fillPlayersPage(int);

    signals:
        void newGameCreated(Game*);

    private:
        // widgety 1. strany
        QSpinBox *m_playersCount;
        QSpinBox *m_signsCountToWin;
        QSpinBox *m_boardSize;
        QLineEdit *m_timeLimit;

        // 2. strana a jej widgety
        QWizardPage *m_playersPage;
        QScrollArea *m_scrollArea;
        QWidget *m_scrollWidget;
        QVector<NewPlayerWidget*> m_newPlayerWidgets;
};

#endif // NEWGAMEDLG_H
