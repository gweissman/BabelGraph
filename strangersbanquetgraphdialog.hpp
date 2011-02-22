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
/* 	FILE:  strangersbanquetgraphdialog.hpp
*  	DESCRIPTION:  Class for a Qt dialog to get input about a strangers' banquet
*					random graph.
*				See http://www.babelgraph.org/about.html for more info about
*				the Strangers' Banquet friendship formation process
*  	AUTHOR: Gary Weissman
*  	DATE: August 2008
*  	CONTACT: gary@babelgraph.org
*	URL: http://www.babelgraph.org/
*/

#ifndef STRANGERSBANQUETGRAPHDIALOGHZ1406_H
#define STRANGERSBANQUETGRAPHDIALOGHZ1406_H

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

class StrangersBanquetGraphDialog : public QDialog
{

	Q_OBJECT
public:
    QDialogButtonBox *buttonBox;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *spinBox;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBox_2;
    QDoubleSpinBox *doubleSpinBox_3;
       QLabel *label_5;
    QSpinBox *spinBox_2;
	QLabel *label_3;

	QWidget *parent;
	StrangersBanquetGraphDialog(QWidget *parentID) { parent = parentID; setupUi(this); }

   void setupUi(QDialog *strangersbanquetGraphDialog)
    {
    if (strangersbanquetGraphDialog->objectName().isEmpty())
        strangersbanquetGraphDialog->setObjectName(QString::fromUtf8("strangersbanquetGraphDialog"));
    strangersbanquetGraphDialog->setWindowModality(Qt::WindowModal);
    strangersbanquetGraphDialog->resize(424, 339);
    strangersbanquetGraphDialog->setMinimumSize(QSize(424, 339));
    strangersbanquetGraphDialog->setMaximumSize(QSize(377, 249));
    strangersbanquetGraphDialog->setLayoutDirection(Qt::LeftToRight);
    strangersbanquetGraphDialog->setModal(false);
    buttonBox = new QDialogButtonBox(strangersbanquetGraphDialog);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(40, 290, 371, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    formLayoutWidget = new QWidget(strangersbanquetGraphDialog);
    formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
    formLayoutWidget->setGeometry(QRect(39, 70, 351, 201));
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

    formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

    spinBox = new QSpinBox(formLayoutWidget);
    spinBox->setObjectName(QString::fromUtf8("spinBox"));
    spinBox->setMinimum(1);
    spinBox->setMaximum(1000);

    formLayout->setWidget(0, QFormLayout::FieldRole, spinBox);

 label_5 = new QLabel(formLayoutWidget);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    formLayout->setWidget(1, QFormLayout::LabelRole, label_5);

    spinBox_2 = new QSpinBox(formLayoutWidget);
    spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
    spinBox_2->setMinimum(1);
    spinBox_2->setMaximum(5);

    formLayout->setWidget(1, QFormLayout::FieldRole, spinBox_2);

    label_4 = new QLabel(formLayoutWidget);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

    doubleSpinBox_2 = new QDoubleSpinBox(formLayoutWidget);
    doubleSpinBox_2->setObjectName(QString::fromUtf8("doubleSpinBox_2"));
    doubleSpinBox_2->setDecimals(3);
    doubleSpinBox_2->setMaximum(1);
    doubleSpinBox_2->setSingleStep(0.01);

    formLayout->setWidget(2, QFormLayout::FieldRole, doubleSpinBox_2);

    doubleSpinBox_3 = new QDoubleSpinBox(formLayoutWidget);
    doubleSpinBox_3->setObjectName(QString::fromUtf8("doubleSpinBox_3"));
    doubleSpinBox_3->setDecimals(3);
    doubleSpinBox_3->setMaximum(1);
    doubleSpinBox_3->setSingleStep(0.01);

    formLayout->setWidget(3, QFormLayout::FieldRole, doubleSpinBox_3);

   

    label_3 = new QLabel(strangersbanquetGraphDialog);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(40, 30, 351, 18));

    retranslateUi(strangersbanquetGraphDialog);
	    retranslateUi(strangersbanquetGraphDialog);
		QObject::connect(buttonBox,SIGNAL(accepted()),this , SLOT(reportInfo()));
    QObject::connect(buttonBox, SIGNAL(accepted()), strangersbanquetGraphDialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), strangersbanquetGraphDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(strangersbanquetGraphDialog);
    } // setupUi

    void retranslateUi(QDialog *strangersbanquetGraphDialog)
    {
    strangersbanquetGraphDialog->setWindowTitle(QApplication::translate("strangersbanquetGraphDialog", "Create a new Strangers' Banquet graph", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("strangersbanquetGraphDialog", "Number of Vertices ", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("strangersbanquetGraphDialog", "Edge Density", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("strangersbanquetGraphDialog", "Discrimination (\316\274)", 0, QApplication::UnicodeUTF8));
	    label_5->setText(QApplication::translate("strangersbanquetGraphDialog", "Number of groups", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("strangersbanquetGraphDialog", "Please select characteristics of a new Strangers' Banquet graph", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(strangersbanquetGraphDialog);
    } // retranslateUi



	signals:
		void strangersbGraphInfo(int,int,float,float);
	private slots:
		void reportInfo()
	{
		emit strangersbGraphInfo(spinBox->value(),spinBox_2->value(),(float)doubleSpinBox_2->value(),(float)doubleSpinBox_3->value());
	}


};


QT_END_NAMESPACE

#endif // STRANGERSBANQUETGRAPHDIALOGHZ1406_H
