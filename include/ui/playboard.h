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

#ifndef PLAYBOARD_H
#define PLAYBOARD_H

#include "main.h"

#include <QTimer>
#include <QWidget>

class Game;
class Player;

// trieda widgetu pre zobrazenie hracej plochy
class PlayBoard : public QWidget {
    Q_OBJECT

    public:
        // vytvori hraciu plochu s rodicom a Game objektom,
        // ktory na nej pracuje
        PlayBoard(QWidget*, Game* = NULL);

        // mozno budu derivati, kt. budu upravovat vzhlad
        virtual ~PlayBoard() { };

        // suradnice na ploche a v poli
        QPoint squareCoords(int, int);
        QPoint arrayCoords(int, int);

        // postara sa o priradenie hracov na plochu
        void mouseReleaseEvent(QMouseEvent*);

        // nastavi premenne urcujuce velkosti atd.
        void resizeEvent(QResizeEvent*);

        // nakresli hraciu plochu
        void paintEvent(QPaintEvent*);

        // vrati a nastavi hru
        Game *game() {
            return m_game;
        };
        const Game *game() const {
            return m_game;
        };

        // zisti ci sa moze klikat
        bool isClickEnabled() const {
            return m_clickEnabled;
        };

        // zisti ci je plocha spustena
        bool isRunning() const {
            return m_isRunning;
        };

    public slots:
        // povoli/zakaze klikanie
        void enableClick(bool v = true) {
            m_clickEnabled = v;
        };
        void disableClick() {
            enableClick(false);
        };

        // sputi/zastavi plochu
        void startBoard();
        void stopBoard();

        // prekresli stvorcek (urceny suradnicami v poli) na ploche
        void repaint(int, int);

        // nastavenie hry
        void setGame(Game*);
        void unsetGame();

    signals:
        // signal je emitnuty, ked bola mys uvolnena
        // nad nejakym stvorcekom a posiela x-ovu a y-ovu suradnicu
        void squareClicked(int, int);

    private:
        // hra, kt. pracuje na ploche
        Game *m_game;

        // urcuje ci sa maju posielat udalosti o kliknutiach
        bool m_clickEnabled;

        // urcuje ci hra bezi
        bool m_isRunning;

        // urcuje velkost vykreslovaneho stvoreceka
        int m_sideSize;

        // urcuje kde sa (ralativne k widgetu) zacina
        // vykreslovat hracia plocha
        int m_fromLeft;
        int m_fromTop;
};

#endif // PLAYBOARD_H
