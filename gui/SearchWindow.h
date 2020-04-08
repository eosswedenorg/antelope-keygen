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
#ifndef SEARCH_WINDOW_H
#define SEARCH_WINDOW_H

#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QGridLayout>
#include <QWidget>
#include <eoskeygen/key_search_result.h>
#include <eoskeygen/key_search.h>

class QThread;

class SearchWindow : public QWidget, public eoskeygen::IKeySearchResult
{
	Q_OBJECT
public:
	explicit SearchWindow(QWidget *parent = 0, Qt::WindowFlags flags = Qt::WindowFlags());
	virtual ~SearchWindow();

	void onResult(const struct libeosio::ec_keypair* key, const struct eoskeygen::KeySearch::result& result);

private :
	void initSignals();

private slots:

	// Start a search
    void search();

	// Output html to screen.
	void output(const std::string& html);
	void output(const QString& html);

	// Called when a search is started.
	void searchStarted();

	// Called when a search is done.
	void searchFinished();

signals:
	void addOutput(const QString& line);

private:

	// Search worker thread.
	QThread* m_worker;

	eoskeygen::KeySearch m_ksearch;

	//  Widgets
	// ----------------

	// Status text.
	QLabel m_status;

	// Search input.
	QLineEdit m_txt_search;

	// Number of Threads.
	QSpinBox m_num_threads;

	// Number of Results
	QSpinBox m_num_results;

	QCheckBox m_leet_cb;

	// Buttons
	QPushButton m_btn_exec;
	QPushButton m_btn_clear;

	// Text output.
	QTextEdit m_output;

	// Gui Layout.
	QGridLayout m_layout;
};

#endif /* SEARCH_WINDOW_H */
