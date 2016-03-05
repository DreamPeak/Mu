/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QLabel>
#include <QBitmap>
#include <QBoxLayout>
#include <QPainter>
#include <QPropertyAnimation>

//Dependencies.
#include "knlocalemanager.h"
#include "knimagelabel.h"
#include "knaccountavatarbutton.h"

//Kreogist Account
#include "account/knaccountpanel.h"
#include "account/knaccountdetails.h"
#include "account/knaccount.h"

#include "knnotificationview.h"
#include "knnotification.h"
#include "knnotificationmodel.h"
#include "knnotificationwidget.h"

#include "knnotificationcenter.h"

KNNotificationCenter::KNNotificationCenter(QWidget *parent) :
    QFrame(parent),
    m_notificationIndicator(new QLabel(this)),
    m_resizeAnime(new QPropertyAnimation(this, "size", this)),
    m_button(new KNAccountAvatarButton(this)),
    m_notificationView(new KNNotificationView(this)),
    m_notificationWidget(new KNNotificationWidget(this)),
    m_accountPanel(new KNAccountPanel(this)),
    m_heightHint(-1)
{
    setObjectName("NotificationCenter");
    //Set properties.
    setAutoFillBackground(true);
    setWindowFlags(Qt::Popup);

    //Configure the indicator.
    m_notificationIndicator->setAutoFillBackground(true);
    m_notificationIndicator->setAttribute(Qt::WA_TransparentForMouseEvents,
                                          true);
    //Set palette.
    QPalette pal=m_notificationIndicator->palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    pal.setColor(QPalette::WindowText, QColor(0,0,0,0));
    pal.setColor(QPalette::AlternateBase, QColor(0,0,0,0));
    pal.setColor(QPalette::ToolTipBase, QColor(255,255,255,0));
    pal.setColor(QPalette::ToolTipText, QColor(255,255,255,0));
    pal.setColor(QPalette::Button, QColor(255,255,255,0));
    pal.setColor(QPalette::ButtonText, QColor(255,255,255,0));
    m_notificationIndicator->setPalette(pal);
    //Set the pixmap.
    QPixmap indicatorPixmap=QPixmap("://public/notification_indicator.png");
    m_notificationIndicator->setPixmap(indicatorPixmap);
    //Configure the indicator.
    m_notificationIndicator->setFixedSize(indicatorPixmap.size());
    m_notificationIndicator->hide();
    //Configure animation.
    m_resizeAnime->setEasingCurve(QEasingCurve::OutCubic);
    //Configure the button.
    m_button->setCursor(Qt::PointingHandCursor);
    m_button->setButtonSize(32);
    //Configure the view.
    m_notificationView->setModel(knNotification->model());
    //Link account avatar change signal with button updater.
    connect(knAccount->accountDetails(), &KNAccountDetails::accountAvatarUpdate,
            [=]
            {
                //Update the notification center button.
                m_button->setAccountAvatar(
                            knAccount->accountDetails()->accountAvatar());
            });
    //Link the account panel to notification.
    connect(m_accountPanel, &KNAccountPanel::requireResize,
            this, &KNNotificationCenter::resizeNotificationCenter);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    setLayout(mainLayout);
    //Add widget to layout.
    mainLayout->addWidget(m_accountPanel);
    mainLayout->addWidget(m_notificationView, 1);

    //Link retranslate.
    knI18n->link(this, &KNNotificationCenter::retranslate);
    retranslate();
}

KNAccountAvatarButton *KNNotificationCenter::headerButton()
{
    return m_button;
}

QWidget *KNNotificationCenter::indicator()
{
    return m_notificationIndicator;
}

int KNNotificationCenter::heightHint()
{
    return m_notificationView->heightHint()+10 + m_accountPanel->height();
}

void KNNotificationCenter::showEvent(QShowEvent *event)
{
    //Show the indicator.
    m_notificationIndicator->show();
    //Do the original show event.
    QFrame::showEvent(event);
}

void KNNotificationCenter::hideEvent(QHideEvent *event)
{
    //Hide the indicator.
    m_notificationIndicator->hide();
    //Do the original hide event.
    QFrame::hideEvent(event);
}

void KNNotificationCenter::resizeEvent(QResizeEvent *event)
{
    //Resize the frame.
    QFrame::resizeEvent(event);
    //Set the rendering.
    QBitmap objBitmap(size());
    QPainter painter(&objBitmap);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform, true);
    painter.fillRect(rect(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    painter.drawRoundedRect(rect(),10,10);
    setMask(objBitmap);
}

void KNNotificationCenter::retranslate()
{
    //Check button state.
    m_button->setToolTip(m_button->isLogin()?
                             tr("Show Kreogist Account details"):
                             tr("Login"));
}

void KNNotificationCenter::resizeNotificationCenter()
{
    //Stop the anime.
    m_resizeAnime->stop();
    //Set the initial size and end size.
    m_resizeAnime->setStartValue(size());
    m_resizeAnime->setEndValue(QSize(width(), qMin(heightHint(),
                                                   maximumHeight())));
    //Start the anime.
    m_resizeAnime->start();
}

KNNotificationWidget *KNNotificationCenter::notificationWidget()
{
    return m_notificationWidget;
}