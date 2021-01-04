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
#include <QIcon>
#include <QLabel>
#include <QGridLayout>
#include <QClipboard>
#include <QGuiApplication>
#include <libeosio/ec.hpp>
#include <libeosio/WIF.hpp>
#include "GenerateWindow.hpp"

void _initKeyWidget(QLineEdit& w) {
	w.setFixedWidth(450);
	w.setReadOnly(true);
}

void _initKeyCopyButton(QPushButton& btn, const QIcon& icon) {
	btn.setFixedWidth(32);
	btn.setIcon(icon);
}

GenerateWindow::GenerateWindow(QWidget *parent) :
QWidget 		(parent),
m_btn_gen		("Generate"),
m_btn_copy_both	("Copy keys")
{
	QIcon copy_icon = QIcon::fromTheme("edit-copy");
	QGridLayout* layout;

	_initKeyWidget(m_pub);
	_initKeyWidget(m_priv);

	_initKeyCopyButton(m_btn_copy_pub, copy_icon);
	_initKeyCopyButton(m_btn_copy_priv, copy_icon);

	// Layout
	layout = new QGridLayout();
	layout->setAlignment(Qt::AlignCenter);

	// Stretch first and last column to make the widgets horizontally centered.
	layout->setColumnStretch(0, 1);
	layout->setColumnStretch(4, 1);

	// Public key row
	layout->addWidget(new QLabel("Public:"), 0, 1, Qt::AlignRight);
	layout->addWidget(&m_pub, 0, 2);
	layout->addWidget(&m_btn_copy_pub, 0, 3);

	// Private key row

	layout->addWidget(new QLabel("Private:"), 1, 1, Qt::AlignRight);
	layout->addWidget(&m_priv, 1, 2);
	layout->addWidget(&m_btn_copy_priv, 1, 3);

	// Bottom row

	m_btn_copy_both.setFixedWidth(80);

	layout->addWidget(&m_btn_gen, 2, 2);
	layout->addWidget(&m_btn_copy_both, 2, 3);

	setLayout(layout);

	// Connections
	connect(&m_btn_gen, SIGNAL(released()), this, SLOT(generate_key()));
	connect(&m_btn_copy_both, SIGNAL(released()), this, SLOT(copy_both_keys()));
	connect(&m_btn_copy_pub, SIGNAL(released()), this, SLOT(copy_pub_key()));
	connect(&m_btn_copy_priv, SIGNAL(released()), this, SLOT(copy_priv_key()));
}

void GenerateWindow::generate_key()
{
	std::string pubstr, privstr;
	struct libeosio::ec_keypair pair;

	libeosio::ec_generate_key(&pair);

	m_pub.setText(QString::fromStdString(libeosio::wif_pub_encode(pair.pub)));
	m_priv.setText(QString::fromStdString(libeosio::wif_priv_encode(pair.secret)));
}

void GenerateWindow::copy_both_keys()
{
	QClipboard *clipboard = QGuiApplication::clipboard();
	clipboard->setText(m_pub.text() + "\n" + m_priv.text());
}

void GenerateWindow::copy_pub_key()
{
	QClipboard *clipboard = QGuiApplication::clipboard();
	clipboard->setText(m_pub.text());
}

void GenerateWindow::copy_priv_key()
{
	QClipboard *clipboard = QGuiApplication::clipboard();
	clipboard->setText(m_priv.text());
}
