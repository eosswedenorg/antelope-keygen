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
#ifndef MULTI_SELECT_H
#define MULTI_SELECT_H

#include <QString>
#include <QStringList>
#include <QPushButton>

class QMouseEvent;
class QPushButton;
class QListWidget;
class QListWidgetItem;
class QDialog;
class MultiSelectDialog;

//
// MultiSelect implements multi selection of items using
// a push button and a dialog with a list of checkbox items.
//
class MultiSelect : public QPushButton
{
    Q_OBJECT
public:
    MultiSelect(const QString& text, QWidget *parent = 0);

	// Items.

	void addItem(const QString& text);

	void addItems(const QStringList& list);

	void clearItems();

	// Get a list of currently selected items.
	QStringList getSelectedItems() const;

signals:

	// This signal is emitted whenever the user has made a new selection.
	void selectionChanged(QStringList selected);

private slots :

	// Called when the dialog is accepted.
	void selectionConfirmed();

	// Called when a list item is clicked on.
	void listItemClicked(QListWidgetItem *item);

protected :

	// Event handlers
	void mousePressEvent(QMouseEvent *e) override;

private :

	// Prefix to show on the button before value.
	QString m_prefix;

	// List of items
	QListWidget* m_list;

	// Dialog to show if this widget are clicked on.
	QDialog* m_dialog;
};

#endif /* MULTI_SELECT_H */
