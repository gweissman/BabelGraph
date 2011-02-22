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
/* 	FILE:  randomgraphdialog.hpp
*  	DESCRIPTION:  Class for a Qt dialog to get input about a random graph
*  	AUTHOR: Gary Weissman
*  	DATE: August 2008
*  	CONTACT: gary@babelgraph.org
*	URL: http://www.babelgraph.org/
*/

#ifndef RANDOMGRAPHDIALOGJ28580_H
#define RANDOMGRAPHDIALOGJ28580_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class RandomGraphDialog : public QDialog
{

	Q_OBJECT

public:
    QDialogButtonBox *buttonBox;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *spinBox;
    QDoubleSpinBox *doubleSpinBox;
    QLabel *label_3;
	QWidget *parent;

	RandomGraphDialog(QWidget *parentID) { parent = parentID; setupUi(this); }

	

    void setupUi(QDialog *randomGraphDialog)
    {
    if (randomGraphDialog->objectName().isEmpty())
        randomGraphDialog->setObjectName(QString::fromUtf8("randomGraphDialog"));
    randomGraphDialog->setWindowModality(Qt::WindowModal);
    randomGraphDialog->resize(377, 249);
    randomGraphDialog->setMinimumSize(QSize(377, 249));
    randomGraphDialog->setMaximumSize(QSize(377, 249));
    randomGraphDialog->setLayoutDirection(Qt::LeftToRight);
    randomGraphDialog->setModal(false);
    buttonBox = new QDialogButtonBox(randomGraphDialog);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(30, 190, 331, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    formLayoutWidget = new QWidget(randomGraphDialog);
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
    spinBox->setMaximum(100000);

    formLayout->setWidget(0, QFormLayout::FieldRole, spinBox);

    doubleSpinBox = new QDoubleSpinBox(formLayoutWidget);
    doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
    doubleSpinBox->setDecimals(5);
    doubleSpinBox->setMinimum(0);
    doubleSpinBox->setMaximum(1);
    doubleSpinBox->setSingleStep(0.01);
    doubleSpinBox->setValue(0);

    formLayout->setWidget(1, QFormLayout::FieldRole, doubleSpinBox);

    label_3 = new QLabel(randomGraphDialog);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(40, 30, 291, 18));

    retranslateUi(randomGraphDialog);


	QObject::connect(buttonBox,SIGNAL(accepted()), this , SLOT(reportInfo()));
    QObject::connect(buttonBox, SIGNAL(accepted()), randomGraphDialog, SLOT(accept()));
	
    QObject::connect(buttonBox, SIGNAL(rejected()), randomGraphDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(randomGraphDialog);
    } // setupUi

    void retranslateUi(QDialog *randomGraphDialog)
    {
    randomGraphDialog->setWindowTitle(QApplication::translate("randomGraphDialog", "Create a new random graph", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("randomGraphDialog", "Number of Vertices ", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("randomGraphDialog", "Edge Density", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("randomGraphDialog", "Please select characteristics of a new random graph:", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(randomGraphDialog);
    } // retranslateUi

	signals:
		void randomGraphInfo(int,double);

	private slots:
		void reportInfo()
		{
			emit randomGraphInfo(spinBox->value(), doubleSpinBox->value());

		}



};

QT_END_NAMESPACE

#endif // RANDOMGRAPHDIALOGJ28580_H
