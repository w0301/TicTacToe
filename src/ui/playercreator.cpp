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

#include "kernel/player.h"

#include "ui/newgamedlg.h"
#include "ui/playersign.h"
#include "ui/playercreator.h"

#include <QtGui>

// PlayerCreatorRegistrator
PlayerCreatorRegistrator::PlayerCreatorsList PlayerCreatorRegistrator::sm_allCreators;

PlayerCreatorRegistrator::PlayerCreatorRegistrator(const QString& name, PlayerCreatorConstructor cnt) {
    sm_allCreators.append( qMakePair(name, cnt) );
}

// PlayerCreator
PlayerCreatorRegistrator PlayerCreator::sm_register(QObject::tr("Local player"), &PlayerCreator::createCreator);

PlayerCreator::PlayerCreator(QWidget *parent) :
        PlayerCreatorBase(parent), m_nameEdit(NULL), m_signType(NULL), m_colorButton(NULL), m_color(Qt::black) {
    // layout pre widget
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    /// pridanie widgetov do layoutu
    // edit pre meno
    mainLayout->addWidget( ( m_nameEdit = new QLineEdit( tr("Name of player") ) ) );
    //mainLayout->addSpacing(1);

    // button na spustenie dialogu pre vyber farby
    QPixmap buttCol(16, 16);
    buttCol.fill(Qt::black);
    mainLayout->addWidget( ( m_colorButton = new QPushButton(QIcon(buttCol), "") ) );
    connect(m_colorButton, SIGNAL(clicked()), this, SLOT(startColorDialog()));

    // box pre vyber podpisu
    mainLayout->addWidget( (m_signType = new QComboBox) );
    m_signType->setCurrentIndex(-1);
    refreshSignTypes();
}

// vytvori noveho hraca podla prislisnych parametrov
Player* PlayerCreator::createPlayer() {
    // vytvorenie objektu podpisu
    PlayerSign *sign = PlayerSignRegistrator::list()[m_signType->currentIndex()].second();
    if(sign->isPainted()) {
        static_cast<PaintedPlayerSign*>(sign)->setColor( color() );
    }

    // vytvorenie hraca a navrat funkcie
    return new Player(sign, m_nameEdit->text());
}

// sloty
void PlayerCreator::refreshSignTypes() {
    // najprv musime list vycistit
    m_signType->clear();

    // a potom ho prislusne naplnime
    PlayerSignRegistrator::SignsList& list = PlayerSignRegistrator::list();
    for(PlayerSignRegistrator::SignsList::const_iterator i = list.begin(); i != list.end(); i++) {
        if(i->first == true) {
            PlayerSign *sign = i->second();
            if(sign->isPainted()) {
                static_cast<PaintedPlayerSign*>(sign)->setColor( color() );
            }
            m_signType->addItem(QIcon(sign->signPixmap(NULL, QPoint(0, 0), 16)), "");
            delete sign;
        }
    }
}

void PlayerCreator::startColorDialog() {
    // vytvorenie dialogu a pripojenie slotov
    QColorDialog colorDialog(this);
    connect(&colorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(setColor(QColor)));
    connect(&colorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(refreshSignTypes()));
    connect(&colorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(changeColorButton(QColor)));
    colorDialog.exec();
}

void PlayerCreator::changeColorButton(QColor col) {
    // zmenenie farby buttonu
    QPixmap buttCol(16, 16);
    buttCol.fill(col);
    m_colorButton->setIcon(QIcon(buttCol));
}
