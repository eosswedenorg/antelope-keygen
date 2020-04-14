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
#include <QDialog>
#include <QLayout>
#include <QMouseEvent>
#include <QListWidget>
#include <QListWidgetItem>
#include "MultiSelect.h"

MultiSelect::MultiSelect(const QString& text, QWidget *parent) :
QPushButton	(text + ": none", parent)
{
	QPushButton* btn;
	QVBoxLayout* layout;

	m_prefix = text;

	// Dialog
	m_dialog = new QDialog(this);

	// Dialog Widgets
	m_list = new QListWidget();
	btn = new QPushButton("Select");

	// Dialog layout
	layout = new QVBoxLayout(m_dialog);
	layout->addWidget(m_list);
	layout->addWidget(btn);

	// Connections
	QObject::connect(btn, SIGNAL(clicked()), m_dialog, SLOT(accept()));
	QObject::connect(m_dialog, SIGNAL(accepted()), this, SLOT(selectionConfirmed()));
	QObject::connect(m_list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(listItemClicked(QListWidgetItem*)));
}

void MultiSelect::addItem(const QString& text, bool checked)
{
	QListWidgetItem* item = new QListWidgetItem(text, m_list);
	item->setFlags(Qt::ItemIsEnabled);
	item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
}

void MultiSelect::addItems(const QStringList& list, bool checked)
{
	QStringList::const_iterator it;

	for(it = list.cbegin(); it != list.cend(); it++) {
		addItem(*it, checked);
	}
}

void MultiSelect::clearItems()
{
	m_list->clear();
}

QStringList MultiSelect::getSelectedItems() const
{
	QListWidgetItem* item;
	QStringList ret;

	// Cannot use m_list->selectedItems() as that function only fetches
	// _selected_ and not _checked_ items (there is a difference).
	for(int i = 0; i < m_list->count(); i++) {
		item = m_list->item(i);
		// Include in list if checked.
		if (item->checkState() == Qt::Checked) {
			ret << item->text();
		}
    }
	return ret;
}

void MultiSelect::selectionConfirmed()
{
	QStringList selected = getSelectedItems();

	// Update the text for this widget to
	// reflect the selected objects.
	QString txt = m_prefix + ": ";

	// Have more than one item. show number of items.
	if (selected.count() > 1) {
		txt += QString::number(selected.count()) + " selected";
	}
	// Just one, we can show the text.
	else if (selected.count() == 1) {
		txt += selected.at(0);
	}
	else {
		txt += "none";
	}

	setText(txt);

	// Emit the selectionChanged signal with the updated list
	emit selectionChanged(selected);
}

void MultiSelect::listItemClicked(QListWidgetItem *item)
{
	// toggle state when user clicks.
	bool checked = item->checkState() == Qt::Checked;
	item->setCheckState(checked ? Qt::Unchecked : Qt::Checked);
}

void MultiSelect::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // Show select dialog.
		m_dialog->show();
    } else {
        // pass on other buttons to base class
        QPushButton::mousePressEvent(event);
    }
}
