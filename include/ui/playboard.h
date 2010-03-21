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

#include <QWidget>
#include <QVector>

class Player;

class PlayBoard : public QWidget {
    Q_OBJECT

    public:
        // vytvori hraciu plochu a znici ju
        PlayBoard(QWidget*, int);
        ~PlayBoard() { };

        // vrati pocet stvorcekov na jednom riadku
        int squareCount() const {
            return m_size;
        };

        // suradnice na ploche a v poli
        QPoint squareCoords(int, int);
        QPoint arrayCoords(int, int);

        // vrati hraca na stvorceku
        Player *playerOn(int x, int y) {
            return m_board[y][x];
        };

        // postara sa o priradenie hracov na plochu
        void mouseReleaseEvent(QMouseEvent*);

        // nastavi premenne urcujuce velkosti atd.
        void resizeEvent(QResizeEvent*);

        // nakresli hraciu plochu
        void paintEvent(QPaintEvent*);

    public slots:
        // sloty daju kamen na plochu
        void putPlayer(int, int);
        void putPlayerOriental(int, int);

    signals:
        // signal je emitnuty, ked bola mys uvolnena
        // nad nejakym stvorcekom a posiela x-ovu a y-ovu suradnicu
        void squareClicked(int, int);

    private:
        // uchovava info o jednotlivich stvorcekoch
        QVector< QVector<Player*> > m_board;

        // uchovava dlzku resp. vysku plochy v stvorcekoch
        int m_size;

        // urcuje velkost vykreslovaneho stvoreceka
        int m_sideSize;

        // urcuje kde sa (ralativne k widgetu) zacina
        // vykreslovat hracia plocha
        int m_fromRight;
        int m_fromTop;
};

#endif // PLAYBOARD_H
