/**
 * MIT License
 *
 * Copyright (c) 2020-2021 EOS Sw/eden
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <QMessageBox>
#include <QMenuBar>
#include <QGridLayout>
#include <QStackedWidget>
#include <libantelope/WIF.hpp>
#include "gui_text.h"
#include "Settings.hpp"
#include "GenerateWindow.hpp"
#include "SearchWindow.hpp"
#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow		(parent),
m_format_fio_action 	(nullptr),
m_format_legacy_action 	(nullptr),
m_format_k1_action	(nullptr)
{
	libantelope::ec_init();

	// Create sub windows and stacked widget.
	m_stacked = new QStackedWidget();
	m_stacked->addWidget(new GenerateWindow());
	m_stacked->addWidget(new SearchWindow());

	setCentralWidget(m_stacked);

	// Add to menu bar.

	menuBar()->addAction("Generate", this, SLOT(switchToGenerate()));
	menuBar()->addAction("Search", this, SLOT(switchToSearch()));

	// Settings

	QActionGroup* formatGroup = new QActionGroup(this);

	m_format_fio_action = new QAction("FIO", formatGroup);
	m_format_fio_action->setCheckable(true);
	m_format_legacy_action = new QAction("Legacy", formatGroup);
	m_format_legacy_action->setCheckable(true);
	m_format_k1_action = new QAction("K1", formatGroup);
	m_format_k1_action->setCheckable(true);

	// Set k1 and trigger the changed action so we set the codec.
	m_format_k1_action->setChecked(true);
	formatK1CheckboxChanged();

	connect(m_format_fio_action, SIGNAL(triggered()), this, SLOT(formatFioCheckboxChanged()));
	connect(m_format_legacy_action, SIGNAL(triggered()), this, SLOT(formatLegacyCheckboxChanged()));
	connect(m_format_k1_action, SIGNAL(triggered()), this, SLOT(formatK1CheckboxChanged()));

	QMenu *settings = menuBar()->addMenu("Settings");
	QMenu *format_menu = settings->addMenu("Key Format");
	format_menu->addAction(m_format_k1_action);
	format_menu->addAction(m_format_legacy_action);
	format_menu->addAction(m_format_fio_action);

	// About
	menuBar()->addAction("About", this, SLOT(showAbout()));
}

MainWindow::~MainWindow()
{
	libantelope::ec_shutdown();
}

void MainWindow::switchToGenerate()
{
	m_stacked->setCurrentIndex(0);
}

void MainWindow::switchToSearch()
{
	m_stacked->setCurrentIndex(1);
}

void MainWindow::showAbout()
{
	QMessageBox::about(this,
		EOSIOKEYGEN_GUI_TEXT_ABOUT_TITLE,
		EOSIOKEYGEN_GUI_TEXT_ABOUT_BODY);
}

void MainWindow::formatFioCheckboxChanged()
{
	if (m_format_fio_action->isChecked()) {
		Settings::setKeyCodec(libantelope::wif_create_legacy_codec("FIO"));
	}
}

void MainWindow::formatLegacyCheckboxChanged()
{
	if (m_format_legacy_action->isChecked()) {
		Settings::setKeyCodec(libantelope::WIF_CODEC_LEG);
	}
}

void MainWindow::formatK1CheckboxChanged()
{
	if (m_format_k1_action->isChecked()) {
		Settings::setKeyCodec(libantelope::WIF_CODEC_K1);
	}
}
