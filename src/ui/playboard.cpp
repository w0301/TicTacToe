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

#include "ui/playboard.h"
#include "ui/playersign.h"

#include <QtGui>


// PlayBoard class
PlayBoard::PlayBoard(Game *game, QWidget *parent) :
        QWidget(parent), m_game(game), m_clickEnabled(false), m_sideSize(0), m_fromLeft(0), m_fromTop(0) {
	if(m_game != NULL) {
		// ked hra zmeni backend plochu musime znova vykreslit
    	connect(m_game, SIGNAL(squareBoardUpdated(int, int)), this, SLOT(repaint(int, int)));

		// ked nastane kliknutie musime poslat hre suradnice stvorceka, kde sa kliklo
    	connect(this, SIGNAL(squareClicked(int, int)), m_game, SLOT(processActualPlayer(int, int)));

        // zabezpecime spravu kliknuti
        connect(m_game, SIGNAL(playerProcessStarted()), this, SLOT(disableClick()));
        connect(m_game, SIGNAL(playerProcessEnded()), this, SLOT(enableClick()));

        // prime signal o vyhre a zastavi plochu
        connect(m_game, SIGNAL(gameEnded(Player*)), this, SLOT(stopBoard()));

        // prime signaly o spusteni/zastaveni hry
        connect(m_game, SIGNAL(gameStarted(Player*)), this, SLOT(startBoard()));
        connect(m_game, SIGNAL(gameEnded()), this, SLOT(stopBoard()));

        if(isRunning()) {
        	startBoard();
		}
    }
}

bool PlayBoard::isRunning() const {
	if(m_game == NULL) {
		return false;
	}
	return m_game->isRunning();
}

void PlayBoard::startBoard() {
    enableClick();
    repaint(WHOLE, WHOLE);
}

void PlayBoard::stopBoard() {
	enableClick(false);
}

// vrati suradnice laveho horneho rohu stvorceka, ktory obsahuje [x, y]
QPoint PlayBoard::squareCoords(int x, int y) {
    if(game() == NULL) {
        return QPoint(-1, -1);
    }

    int wh = game()->squareBoardSize() * m_sideSize;
    QRect boardRect(m_fromLeft, m_fromTop, wh, wh);
    if(boardRect.contains(x, y)) {
        // prevedie suradnice v stvorceku na suradnice v poli
        // nastavi hraca
        return QPoint( ((x - m_fromLeft) / m_sideSize) * m_sideSize, ((y - m_fromTop) / m_sideSize) * m_sideSize);
    }
    return QPoint(-1, -1);
}

// vrati x a y suradnicu (zabalenu v QPoint) v poli patraciu k predanemu bodu
QPoint PlayBoard::arrayCoords(int x, int y) {
    QPoint corner = squareCoords(x, y);
    if(corner == QPoint(-1, -1)) {
        return QPoint(-1, -1);
    }
    return QPoint(corner.x() / m_sideSize, corner.y() / m_sideSize);
}

// prekresli jeden stvorcek
void PlayBoard::repaint(int x, int y) {
    if(x != WHOLE && y != WHOLE) {
        QWidget::repaint( QRect( QPoint(x * m_sideSize + m_fromLeft, y * m_sideSize + m_fromTop), QSize(m_sideSize, m_sideSize) ) );
    }
    else {
        QWidget::repaint();
    }
}

// spracovanie eventov
void PlayBoard::mouseReleaseEvent(QMouseEvent *event) {
    // mame posielat spravu o kliknuti?
    if(!isClickEnabled() || !isRunning()) {
        return;
    }

    // suradnice kliknutia resp. uvolnenia mysi
    int x = event->x();
    int y = event->y();

    QPoint arrCoords = arrayCoords(x, y);

    if(arrCoords != QPoint(-1, -1)) {
        // posle signal o tom, ze doslo ku kliknutiu na plochu
        emit squareClicked(arrCoords.x(), arrCoords.y());
    }
}

void PlayBoard::resizeEvent(QResizeEvent*) {
    if(game() == NULL) {
        return;
    }

    // pocet stvorcekov na strane
    int sqCount = game()->squareBoardSize();

    // budeme carovat s velkostou widgetu, aby sme
    // nastavili velkost 1 stvorceka na ploche
    m_sideSize = qMin(width() / sqCount, height() / sqCount);

    // postara sa o vycentrovanie plochy na widgete
    m_fromTop = (height() - m_sideSize * sqCount) / 2;
    m_fromLeft = (width() - m_sideSize * sqCount) / 2;
}

void PlayBoard::paintEvent(QPaintEvent*) {
    if(game() == NULL) {
        return;
    }

    // nas kresliacy nastroj :)
    QPainter painter(this);

    // pocet stvorcekov na strane
    int sqCount = game()->squareBoardSize();

    for(int y = 0; y != sqCount; y++) {
        for(int x = 0; x != sqCount; x++) {
            // vykreslenie prazdneho stvorceka
            painter.drawRect(x*m_sideSize + m_fromLeft, y*m_sideSize + m_fromTop, m_sideSize, m_sideSize);

            // tu pride vykreslenie kruzkov, krizikov atd.
            if(game()->square(x, y) != NULL) {
                int squareX = (x*m_sideSize + m_fromLeft) + 1;
                int squareY = (y*m_sideSize + m_fromTop) + 1;
                painter.drawPixmap(squareX, squareY, game()->square(x, y)->sign()->signPixmap(this, QPoint(squareX, squareY), m_sideSize - 1));

                // ked su stvorceky vytazne preciarkneme ich
                if(game()->square(x, y).isWinning()) {
                    QPen oldPen = painter.pen();
                    QPen newPen(QColor(255, 255, 255));
                    newPen.setWidth(3);

                    painter.setPen(newPen);
                    painter.drawLine(squareX, squareY, squareX + m_sideSize, squareY + m_sideSize);
                    painter.setPen(oldPen);
                }
            }
        }
    }
}
