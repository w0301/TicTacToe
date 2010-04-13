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
#include <QWizard>


class Game;
class PlayerCreatorBase;

class QLabel;
class QSpinBox;
class QLineEdit;
class QComboBox;
class QWizardPage;
class QVBoxLayout;

// NewPlayerWidget class
class NewPlayerWidget : QWidget {
    Q_OBJECT

    public:
        NewPlayerWidget(QWidget* = NULL);
        ~NewPlayerWidget() { };

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
        typedef PlayerCreatorBase *(*PlayerCreatorConstructor)();
        typedef QVector< QPair<QString, PlayerCreatorConstructor> > PlayerCreatorsList;

        // vytvori dialog
        NewGameDialog(QWidget* = NULL);

        // znici dialog
        ~NewGameDialog();

        // vytvori jednotlive strany
        QWizardPage *createInitialPage();
        QWizardPage *createPlayersPage();

        // sprava listu createrov
        static void registerCreator(const QString& name, PlayerCreatorConstructor cnt) {
            m_playerCreators.push_back( QPair<QString, PlayerCreatorConstructor>(name, cnt) );
        };
        static PlayerCreatorsList& creatorsList() {
            return m_playerCreators;
        };

    public slots:
        // vytvori hru a posle ju signalom newGameCreated
        void createNewGame();

    signals:
        void newGameCreated(Game*);

    private:
        // widgety 1. strany
        QSpinBox *m_playersCount;
        QSpinBox *m_signsCountToWin;
        QSpinBox *m_boardSize;
        QLineEdit *m_timeLimit;

        // zoznam vsetkych tried, ktore vytvaraju hracov
        static PlayerCreatorsList m_playerCreators;
};

#endif // NEWGAMEDLG_H
