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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QPainter>
#include <QSizePolicy>
#include <QMouseEvent>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

#include "knopacityanimebutton.h"

KNOpacityAnimeButton::KNOpacityAnimeButton(QWidget *parent) :
    KNAbstractButton(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    //Initial the graphic effects.
    m_effect=new QGraphicsOpacityEffect(this);
    m_effect->setOpacity(m_startOpacity);
    setGraphicsEffect(m_effect);

    //Initial the animations.
    m_mouseDown=new QPropertyAnimation(this);
    m_mouseDown->setEndValue(1.0);
    configureAnimation(m_mouseDown);

    m_mouseIn=new QPropertyAnimation(this);
    m_mouseIn->setEndValue(m_inoutOpacity);
    configureAnimation(m_mouseIn);

    m_mouseUp=new QPropertyAnimation(this);
    m_mouseUp->setEndValue(m_inoutOpacity);
    configureAnimation(m_mouseUp);

    m_mouseOut=new QPropertyAnimation(this);
    m_mouseOut->setEndValue(m_startOpacity);
    configureAnimation(m_mouseOut);
}

void KNOpacityAnimeButton::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    //Stop in out animations.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set mouse in parameters.
    m_mouseIn->setStartValue(m_effect->opacity());
    m_mouseIn->start();
}

void KNOpacityAnimeButton::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    //Stop in out animations.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set mouse out parameters.
    m_mouseOut->setStartValue(m_effect->opacity());
    m_mouseOut->start();
}

void KNOpacityAnimeButton::mousePressEvent(QMouseEvent *event)
{
    //Stop in out animations.
    m_mouseUp->stop();
    m_mouseDown->stop();
    if(event->button()==Qt::LeftButton)
    {
        //Set mouse down flags.
        m_isPressed=true;
        //Set mouse down parameters.
        m_mouseDown->setStartValue(m_effect->opacity());
        m_mouseDown->start();
    }
    QWidget::mousePressEvent(event);
}

void KNOpacityAnimeButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Stop in out animations.
    m_mouseDown->stop();
    m_mouseUp->stop();
    //Ensure this button has been mouse down.
    if(m_isPressed &&
            event->button()==Qt::LeftButton &&
            rect().contains(event->pos()))
    {
        //Accept event.
        event->accept();
        //Start mouse up animation.
        m_mouseUp->setStartValue(m_effect->opacity());
        m_mouseUp->start();
        //Emit clicked signals.
        emit clicked();
    }
    m_isPressed=false;
    QWidget::mouseReleaseEvent(event);
}

void KNOpacityAnimeButton::configureAnimation(QPropertyAnimation *animation)
{
    animation->setTargetObject(m_effect);
    animation->setPropertyName("opacity");
    animation->setDuration(100);
    animation->setEasingCurve(QEasingCurve::OutCubic);
}
QPixmap KNOpacityAnimeButton::icon() const
{
    return m_icon;
}

QSize KNOpacityAnimeButton::sizeHint() const
{
    return m_icon.size();
}

void KNOpacityAnimeButton::setIcon(const QPixmap &icon)
{
    m_icon=icon;
    //Repaint the widget mannally.
    update();
}

void KNOpacityAnimeButton::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    //Initial the painter.
    QPainter painter(this);
    //Set painter render hints.
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    //Draw pixmap.
    painter.drawPixmap(0,0,m_icon.scaled(size(),
                                         Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation));
}
