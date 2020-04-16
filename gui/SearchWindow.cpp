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
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>
#include <QGridLayout>
#include <QFuture>
#include <QtConcurrent>
#include <libeosio/WIF.h>
#include <eoskeygen/core/leet.h>
#include <eoskeygen/core/string.h>
#include "config.hpp"
#include "helpers.hpp"
#include "SearchWindow.hpp"

SearchWindow::SearchWindow(QWidget *parent, Qt::WindowFlags flags) :
QWidget			(parent, flags),
m_status		("status"),
m_leet_cb		("L33t"),
m_dict_lang 	("Dictionary Language"),
m_dict_file 	("Dictionary File", true),
m_btn_exec		("Search"),
m_btn_clear		("Clear")
{
	setMinimumSize(600, 400);

	// Monospaced font
	QFont f_mono("monospace");

	// Output
	m_output.setFont(f_mono);
	m_output.setReadOnly(true);

	// Layout
	// ------------------------
	setLayout(&m_layout);
	m_layout.setColumnStretch(0, 10);
	m_layout.setColumnStretch(1, 10);

	// First row.
	m_dict_lang.addItems(get_files(CONFIG_DICT_FULL_PATH));
	m_layout.addWidget(&m_dict_lang, 0, 0);
	m_layout.addWidget(&m_dict_file, 0, 1);

	m_layout.addWidget(&m_leet_cb, 0, 2);

	m_num_threads.setValue((int) eoskeygen::KeySearch::max_threads());
	m_num_threads.setRange(1, (int) eoskeygen::KeySearch::max_threads());
	m_num_threads.setSuffix(" Threads");
	m_layout.addWidget(&m_num_threads, 0, 3);

	m_num_results.setValue(10);
	m_num_results.setRange(1, 99);
	m_num_results.setSuffix(" Results");
	m_layout.addWidget(&m_num_results, 0, 4);

	// Second row.
	m_layout.addWidget(&m_status, 1, 0, 1, 3);
	m_layout.addWidget(&m_txt_search, 1, 0, 1, 3);
	m_layout.addWidget(&m_btn_exec, 1, 3);
	m_layout.addWidget(&m_btn_clear, 1, 4);

	// Third row.
	m_layout.addWidget(&m_output, 2, 0, 1, 0);

	// Search
	// ------------------------

	m_ksearch.setCallback(this);

	initSignals();

	// Focus search field.
	m_txt_search.setFocus();
}

void SearchWindow::initSignals()
{
	// Buttons
	connect(&m_btn_exec, SIGNAL(released()), this, SLOT(search()));
	connect(&m_btn_clear, SIGNAL(released()), &m_output, SLOT(clear()));

	// Worker Thread
	connect(&m_worker, SIGNAL(started()), this, SLOT(searchStarted()));
	connect(&m_worker, SIGNAL(finished()), this, SLOT(searchFinished()));

	connect(this, SIGNAL(addOutput(QString)), this, SLOT(output(QString)));

	connect(&m_dict_file, SIGNAL(addNewItem()), this, SLOT(langFileAdd()));
}

void SearchWindow::loadDictionaries()
{
	QStringList list;
	eoskeygen::Dictionary tmpDict;
	std::string base_path(CONFIG_DICT_FULL_PATH);

	// Clear dictionary first.
	m_dict.clear();

	// Go through all selected languages.
	list = m_dict_lang.getSelectedItems();
	for(QStringList::const_iterator it = list.cbegin(); it != list.cend(); it++) {

		// Load and add them to dictionary.
		tmpDict.loadFromFile(base_path + "/" + it->toStdString());
		m_dict.add(tmpDict);
	}

	// Go through all selected files.
	list = m_dict_file.getSelectedItems();
	for(QStringList::const_iterator it = list.cbegin(); it != list.cend(); it++) {

		// Load and add them to dictionary.
		tmpDict.loadFromFile(it->toStdString());
		m_dict.add(tmpDict);
	}
}

void SearchWindow::onResult(const struct libeosio::ec_keypair* key, const struct eoskeygen::KeySearch::result& result)
{
	int pos = (int) result.pos;
	int len = (int) result.len;
 	QString pub = QString::fromStdString(libeosio::wif_pub_encode(key->pub));
	QString mid = pub.mid(pos, len);
	QString left = pub.left(pos);
	QString right = pub.mid(pos + len, pub.size() - pos);
	eoskeygen::Dictionary::search_result_t dict_res = m_dict.search(pub.toStdString());

	QString out = "Public:  " + pub.left(3);
	for(int i = 3; i < pub.length(); ) {

		if (i == pos) {
			out += "<font color=red>" + pub.mid(pos, len) + "</font>";
			i += len;
			continue;
		}

		// Look in the dictionary.
		auto dp = dict_res.find(i);
		if (dp != dict_res.end()) {
			int p = (int) dp->first;
			int l = (int) dp->second;
			out += "<font color=blue>" + pub.mid(p, l) + "</font>";
			i += l;
			continue;
		}

		out += pub[i++];
	}

	out += "<br/>Private: " + QString::fromStdString(libeosio::wif_priv_encode(key->secret));

	// As this function could be called from a non-gui thread. we use signals.
	emit addOutput("<p>" + out + "</p>");
}

// --------------------
//  Slots
// --------------------

void SearchWindow::search()
{
	if (m_worker.isRunning()) {
		return;
	}

	const std::string& input = m_txt_search.text().toLocal8Bit().constData();
	eoskeygen::strlist_t list;

	if (m_leet_cb.isChecked()) {
		list = eoskeygen::l33twords(input);
	} else {
		list = eoskeygen::strlist::splitw(input);
	}

	// Validate that we atleast got something to search for.
	if (list.size() < 1 || (list.size() == 1 && list[0] == "")) {
		QMessageBox::warning( this,
	    	"Empty search field.",
	    	"You must specify atleast one search string" );
		return;
	}

	loadDictionaries();

	m_ksearch.clear();
	m_ksearch.addList(list);
	m_ksearch.setThreadCount(m_num_threads.value());

	QFuture<void> future = QtConcurrent::run(m_ksearch, &eoskeygen::KeySearch::find, m_num_results.value());
	m_worker.setFuture(future);

	m_status.setText("Searching for: " + QString::fromStdString(eoskeygen::strlist::join(list, ", ")));
}

void SearchWindow::output(const std::string& html)
{
	output(QString::fromStdString(html));
}

void SearchWindow::output(const QString& html)
{
	if (m_output.toPlainText().size()) {
		m_output.setHtml(m_output.toHtml() + html);
	} else {
		m_output.setHtml(html);
	}

	// Force scrollbar to the bottom.
	m_output.verticalScrollBar()->setValue(m_output.verticalScrollBar()->maximum());
}

void SearchWindow::langFileAdd()
{
	QStringList files = QFileDialog::getOpenFileNames(this,
		"Select one or more language files");

	m_dict_file.addItems(files, true);
}

void SearchWindow::searchStarted()
{
	m_txt_search.setEnabled(false);
	m_txt_search.setHidden(true);
	m_dict_lang.setEnabled(false);
	m_dict_file.setEnabled(false);
	m_leet_cb.setEnabled(false);
	m_btn_exec.setEnabled(false);
	m_btn_clear.setEnabled(false);
	m_num_threads.setEnabled(false);
	m_num_results.setEnabled(false);
}

void SearchWindow::searchFinished()
{
	m_txt_search.setEnabled(true);
	m_txt_search.setHidden(false);
	m_dict_lang.setEnabled(true);
	m_dict_file.setEnabled(true);
	m_leet_cb.setEnabled(true);
	m_btn_exec.setEnabled(true);
	m_btn_clear.setEnabled(true);
	m_num_threads.setEnabled(true);
	m_num_results.setEnabled(true);
}
