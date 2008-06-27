/*******************************************************************
 *
 * Copyright 2008 Sascha Peilicke <sasch.pe@gmx.de>
 *
 * This file is part of the KDE project "KWater Screen Saver"
 *
 * KGo is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * KGo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KReversi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *******************************************************************
 */

/** @file
 * This file is part of KDE Water screensaver.
 *
 * @author Sascha Peilicke <sasch.pe@gmx.de>
 */
#include "kwatergl.h"

#include <cmath>
#include <QDesktopWidget>
#include <KApplication>
#include <KDebug>

	/*, m_desktopPixmap(QPixmap::grabWindow(KApplication::desktop()->winId()))
	, m_cur(&m_water1)
	, m_old(&m_water2)*/

/*void KWaterScreenSaver::resizeEvent(QResizeEvent *event)
{
	m_backgroundImage = m_desktopPixmap.scaled(event->size()).toImage();
	m_waterImage = QImage(event->size(), m_backgroundImage.format());
	m_water1.resize(event->size().width() * event->size().height());
	m_water1.fill(0);
	m_water2.resize(m_water1.size());
	m_water2.fill(0);
}

void KWaterScreenSaver::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	painter.drawImage(rect(), m_waterImage);

	// Flip the pointer to the current water pixmap
	QVector<int> *tmp = m_cur;
	m_cur = m_old;
	m_old = tmp;
}

void KWaterScreenSaver::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
		m_old->replace((event->y() + 1) * width() + event->x() + 1, -120);
		m_old->replace((event->y() + 1) * width() + event->x(), -160);
		m_old->replace((event->y() + 1) * width() + event->x() - 1, -120);
		m_old->replace(event->y() * width() + event->x() + 1, -160);
		m_old->replace(event->y() * width() + event->x(), -200);
		m_old->replace(event->y() * width() + event->x() - 1, -160);
		m_old->replace((event->y() - 1) * width() + event->x() + 1, -120);
		m_old->replace((event->y() - 1) * width() + event->x(), -160);
		m_old->replace((event->y() - 1) * width() + event->x() - 1, -120);
	}
}

void KWaterScreenSaver::timeout()
{
	// Computer water waves
	for (int y = 1; y < height() - 1; y++) {
		for (int x = 1; x < width() - 1; x++) {
			int index = y * width() + x;
			int value = (m_old->value(index - 1) + 
				m_old->value(index + 1) +
				m_old->value(index - width()) +
				m_old->value(index + width())) / 2 - m_cur->value(index);
			value -= value >> 16;
			m_cur->replace(index, value);
		}
	}
	// Compute transparent surface ray-tracing
	for (int y = 1; y < height() - 1; y++) {
		for (int x = 1; x < width() - 1; x++) {
			int index = y * width() + x;
			int xDiff = m_old->value(index + 1) - m_old->value(index);
			int yDiff = m_old->value(index + width()) - m_old->value(index);

			double xAngle = atan(xDiff);
			double xRefraction = asin(sin(xAngle) / 2.0);
			double xDisplace = trunc(tan(xRefraction) * xDiff);

			double yAngle = atan(yDiff);
			double yRefraction = asin(sin(yAngle) / 2.0);
			double yDisplace = trunc(tan(yRefraction) * yDiff);

			int newColor;
			if (xDiff < 0) {
				// Current position is higher - Clockwise rotation
				if (yDiff < 0)
					newColor = m_backgroundImage.pixel(x - xDisplace, y - yDisplace);
				else
					newColor = m_backgroundImage.pixel(x - xDisplace, y + yDisplace);
			} else {
				// Current position is lower - Counterclockwise rotation
				if (yDiff < 0)
					newColor = m_backgroundImage.pixel(x + xDisplace, y - yDisplace);
				else
					newColor = m_backgroundImage.pixel(x + xDisplace, y + yDisplace);
			}
			m_waterImage.setPixel(x, y, newColor);
		}
	}
	update();
}*/

KWaterGLWidget::KWaterGLWidget(QWidget *parent)
	: QGLWidget(parent)
	, m_desktopPixmap(QPixmap::grabWindow(KApplication::desktop()->winId()))
{
}

KWaterGLWidget::~KWaterGLWidget()
{
}

void KWaterGLWidget::initializeGL()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
}

void KWaterGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();

	glTranslate(-1.5f, 0.0f, -6.0f);
	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 0.0f,  1.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glCOlor3f(0.0f, 0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
	glEnd();

	glTranslate(3.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f( 1.0f, 1.0f, 0.0f);
		glVertex3f( 1.0f,-1.0f, 0.0f);
		glVertex3f(-1.0f,-1.0f, 0.0f);
	glEnd();

	glFlush();
}

void KWaterGLWidget::resizeGL(int width, int height)
{
	if (height == 0)
		heigth = 1;

	glViewport(0, 0, width, height);// Reset the current viewport
	glMatrixMode(GL_PROJECTION);	// Select the projection matrix
	glLoadIdentity();				// And reset it
	//gluPerspective(45.0f, (GLFloat)width / (GLFloat)height, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);		// Select the modelview matrix
	glLoadIdentity();				// And reset it
}

//----------------------------------------------------------------------------


KWaterScreenSaver::KWaterScreenSaver(WId id)
	: KScreenSaver(id)
	m_water(KWaterGLWidget())
{
	readSettings();
	embed(m_water);
	m_water->show();
	connect(&m_timer, SIGNAL(timeout()), m_water, SLOT(updateGL()));
	m_timer.start(25);
}

KWaterScreenSaver::~KWaterScreenSaver()
{
	m_timer.stop();
}

void KWaterScreenSaver::readSettings()
{
	//KConfigGroup config(KGlobal::config(), "Settings");
	// ....
}

//----------------------------------------------------------------------------

KWaterSetup::KWaterSetup(QWidget *parent)
	: KDialog(parent)
{
	// configure amount of drops, drop size/strength
	// water color ...
}

KWaterSetup::~KWaterSetup()
{
}

void KWaterSetup::updateSettings()
{
}

void KWaterSetup::readSettings()
{
}

//----------------------------------------------------------------------------
// libkscreensaver interface implementation
//----------------------------------------------------------------------------

class KWaterScreenSaverInterface : public KScreenSaverInterface
{
public:
	virtual KAboutData *aboutData() {
		return new KAboutData("kwater.kss", 0, ki18n("Water"), "0.1", ki18n("Water Screensaver"), KAboutData::License_GPL);
	}
	
	virtual KScreenSaver *create(WId id) {
		return new KWaterScreenSaver(id);
	}

	virtual QDialog *setup() {
		return new KWaterSetup();
	}
};

int main(int argc, char *argv[])
{
	KWaterScreenSaverInterface kss;
	return kScreenSaverMain(argc, argv, kss);
}
