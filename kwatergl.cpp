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
#include <QTimer>
#include <KApplication>
#include <KDebug>

KWaterGLWidget::KWaterGLWidget(QWidget *parent)
	: QGLWidget(parent)
	, m_desktopPixmap(QPixmap::grabWindow(KApplication::desktop()->winId()))
{
}

KWaterGLWidget::~KWaterGLWidget()
{
	glDetachShader(m_p, m_vs1);
	glDetachShader(m_p, m_fs1);

	glDeleteShader(m_vs1);
	glDeleteShader(m_fs1);
	glDeleteProgram(m_p);
}

void KWaterGLWidget::initializeGL()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	const GLchar *vs1_source = 
		"varying vec3 normal, lightDir;"
		"void main() {"
		"    lightDir = normalize(vec3(gl_LightSource[0].position);"
		"    normal = normalize(gl_NormalMatrix * gl_Normal);"
		"    gl_Position = ftransform();"
		"}";
	const GLchar *fs1_source = 
		"varying vec3 normal, lightDir;"
		"void main() {"
		"    float intensity;"
		"    vec3 n;"
		"    vec4 color;"
		"    n = normalize(normal);"
		"    intensity = max(dot(lightDir, n), 0.0);"
		"    if (intensity > 0.98)"
		"        color = vec4(0.8, 0.8, 0.8, 1.0);"
		"    else if (intensity > 0.5)"
		"        color = vec4(0.4, 0.4, 0.8, 1.0);"
		"    else if (intensity > 0.25)"
		"        color = vec4(0.2, 0.2, 0.4, 1.0);"
		"    else"
		"        color = vec4(0.1, 0.1, 0.1, 1.0);"
		"    gl_FragColor = color;"
		"}";

	m_vs1 = glCreateShader(GL_VERTEX_SHADER);
	m_fs1 = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(m_vs1, 1, &vs1_source, 0);
	glShaderSource(m_fs1, 1, &fs1_source, 0);

	glCompileShader(m_vs1);
	glCompileShader(m_fs1);

	m_p = glCreateProgram();
	glAttachShader(m_p, m_vs1);
	glAttachShader(m_p, m_fs1);
	glLinkProgram(m_p);
	
	glUseProgram(m_p);
}

void KWaterGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();

	glTranslatef(-1.5f, 0.0f, -6.0f);
	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 0.0f,  1.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
	glEnd();

	glTranslatef(3.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f( 1.0f, 1.0f, 0.0f);
		glVertex3f( 1.0f,-1.0f, 0.0f);
		glVertex3f(-1.0f,-1.0f, 0.0f);
	glEnd();

	glFlush();
	QTimer::singleShot(25, this, SLOT(updateGL()));
}

void KWaterGLWidget::resizeGL(int width, int height)
{
	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);// Reset the current viewport
	glMatrixMode(GL_PROJECTION);	// Select the projection matrix
	glLoadIdentity();				// And reset it
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);		// Select the modelview matrix
	glLoadIdentity();				// And reset it
}

/*, m_desktopPixmap(QPixmap::grabWindow(KApplication::desktop()->winId()))
, m_cur(&m_water1)
, m_old(&m_water2)

void KWaterScreenSaver::resizeEvent(QResizeEvent *event)
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

//----------------------------------------------------------------------------


KWaterScreenSaver::KWaterScreenSaver(WId id)
	: KScreenSaver(id)
{
	readSettings();
	embed(&m_water);
	m_water.show();
	QTimer::singleShot(25, &m_water, SLOT(updateGL()));
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
