/**
 * MIT License
 *
 * Copyright (c) 2020 EOS Sw/eden
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
#ifndef GENERATE_WINDOW_H
#define GENERATE_WINDOW_H

#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class GenerateWindow : public QWidget
{
	Q_OBJECT
public:
    GenerateWindow(QWidget *parent = 0);

public slots:

	// Genereate a new key.
    void generate_key();

	// Copy both keys to clipboard
	void copy_both_keys();

	// copy public key to clipboard
	void copy_pub_key();

	// copy private key to clipboard
	void copy_priv_key();

protected:

	QLineEdit m_pub;
	QLineEdit m_priv;

	QPushButton m_btn_gen;
	QPushButton m_btn_copy_both;
	QPushButton m_btn_copy_priv;
	QPushButton m_btn_copy_pub;
};

#endif /* GENERATE_WINDOW_H */
