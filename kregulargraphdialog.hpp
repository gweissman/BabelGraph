/*******************************************************************************
*     This program is free software: you can redistribute it and/or modify     *
*     it under the terms of the GNU General Public License as published by     *
*     the Free Software Foundation, either version 3 of the License, or        *
*     (at your option) any later version.                                      *
*                                                                              *
*     This program is distributed in the hope that it will be useful,          *
*     but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*     GNU General Public License for more details.                             *
*                                                                              *
*     You should have received a copy of the GNU General Public License        *
*     along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
********************************************************************************/
/* 	FILE:  kregulargraphdialog.hpp
*  	DESCRIPTION:  Class for a Qt dialog to get input about a k-regular graph
*  	AUTHOR: Gary Weissman
*  	DATE: August 2008
*  	CONTACT: gary@babelgraph.org
*	URL: http://www.babelgraph.org/
*/

#ifndef KREGULARGRAPHDIALOGH30505_H
#define KREGULARGRAPHDIALOGH30505_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class KRegularGraphDialog : public QDialog
{
	Q_OBJECT
public:
    QDialogButtonBox *buttonBox;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *spinBox;
    QSpinBox *spinBox_2;
    QLabel *label_3;

		QWidget *parent;

		KRegularGraphDialog(QWidget *parentID) { parent = parentID; setupUi(this); }

    void setupUi(QDialog *kregularGraphDialog)
    {
    if (kregularGraphDialog->objectName().isEmpty())
        kregularGraphDialog->setObjectName(QString::fromUtf8("kregularGraphDialog"));
    kregularGraphDialog->setWindowModality(Qt::WindowModal);
    kregularGraphDialog->resize(377, 249);
    kregularGraphDialog->setMinimumSize(QSize(377, 249));
    kregularGraphDialog->setMaximumSize(QSize(377, 249));
    kregularGraphDialog->setLayoutDirection(Qt::LeftToRight);
    kregularGraphDialog->setModal(false);
    buttonBox = new QDialogButtonBox(kregularGraphDialog);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(30, 190, 331, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    formLayoutWidget = new QWidget(kregularGraphDialog);
    formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
    formLayoutWidget->setGeometry(QRect(39, 70, 291, 101));
    formLayout = new QFormLayout(formLayoutWidget);
    formLayout->setObjectName(QString::fromUtf8("formLayout"));
    formLayout->setHorizontalSpacing(5);
    formLayout->setVerticalSpacing(23);
    formLayout->setContentsMargins(0, 0, 0, 0);
    label = new QLabel(formLayoutWidget);
    label->setObjectName(QString::fromUtf8("label"));

    formLayout->setWidget(0, QFormLayout::LabelRole, label);

    label_2 = new QLabel(formLayoutWidget);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

    spinBox = new QSpinBox(formLayoutWidget);
    spinBox->setObjectName(QString::fromUtf8("spinBox"));
    spinBox->setMinimum(1);
    spinBox->setMaximum(1000);

    formLayout->setWidget(0, QFormLayout::FieldRole, spinBox);

    spinBox_2 = new QSpinBox(formLayoutWidget);
    spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));

    formLayout->setWidget(1, QFormLayout::FieldRole, spinBox_2);

    label_3 = new QLabel(kregularGraphDialog);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(40, 30, 291, 18));

    retranslateUi(kregularGraphDialog);
		QObject::connect(buttonBox,SIGNAL(accepted()), this , SLOT(reportInfo()));
    QObject::connect(buttonBox, SIGNAL(accepted()), kregularGraphDialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), kregularGraphDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(kregularGraphDialog);
    } // setupUi

    void retranslateUi(QDialog *kregularGraphDialog)
    {
    kregularGraphDialog->setWindowTitle(QApplication::translate("kregularGraphDialog", "Create a new k-regular graph", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("kregularGraphDialog", "Number of Vertices ", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("kregularGraphDialog", " k-value", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("kregularGraphDialog", "Please select characteristics of a new k-regular graph:", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(kregularGraphDialog);
    } // retranslateUi


	signals:
		void kRegularGraphInfo(int,int);

	private slots:
		void reportInfo()
		{
			emit kRegularGraphInfo(spinBox->value(), spinBox_2->value());

		}



};


QT_END_NAMESPACE

#endif // KREGULARGRAPHDIALOGH30505_H
