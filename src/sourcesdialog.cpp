/*
 *  VarB - genomic analysis software
 *
 *  software.markdpreston.com/varb
 *
 *  (c) Mark D Preston, H Magnus Manske 2011-
 *
 *  When using this software, commercially or academically, please
 *  get in contact and reference:
 *
 *  Mark D Preston, H Magnus Manske, et al.
 *  VarB: A Variation Browsing tool for polymorphisms derived from
 *  next-generation sequence data
 *  In submission
 *
 *
 *  This software was written, in large part, at the London School of
 *  Hygiene and Tropical Medicine, UK and the Sanger Institute
 *  Cambridge, UK.
 *
 *
 *  This file is part of VarB.
 *
 *  VarB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  VarB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with VarB.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "sourcesdialog.h"
#include "ui_sourcesdialog.h"
#include "mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>

SourcesDialog::SourcesDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SourcesDialog) {
    ui->setupUi(this);
    connect(ui->infoButton, SIGNAL(clicked()), this, SLOT(on_infoButton_clicked()));
}

SourcesDialog::~SourcesDialog() {
    delete ui;
}

void SourcesDialog::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SourcesDialog::initialise(QString seq, QString anno, QString var, QString filter) {
    ui->annoLine->setText(anno);
    ui->referenceLine->setText(seq);
    ui->varLine->setText(var);
    ui->filterLine->setText(filter);
}

void SourcesDialog::on_referenceButton_clicked() {
    QString f = QFileDialog::getOpenFileName(this, tr("Select local reference file"), ui->referenceLine->text(), tr("FASTA Files (*.fa *.fasta);;All Files (*)"));
    if (!f.isNull() && f != ui->referenceLine->text()) {
        ui->referenceLine->setText(f);
    }
}

void SourcesDialog::on_annoButton_clicked() {
    QString f = QFileDialog::getOpenFileName(this, tr("Select local annotation file"), ui->annoLine->text(), tr("GFF Files (*.gff);;All Files (*)"));
    if (!f.isNull() && f != ui->annoLine->text()) {
        ui->annoLine->setText(f);
    }
}

void SourcesDialog::on_varButton_clicked() {
    QString f = QFileDialog::getOpenFileName(this, tr("Select local variation file"), ui->varLine->text(), tr("VCF Files (*.vcf);;All Files (*)"));
    if (!f.isNull() && f != ui->varLine->text()) {
        ui->varLine->setText(f);
    }
}

void SourcesDialog::on_okButton_clicked() {
    close();
    MainWindow::instance->loadData(ui->referenceLine->text(), ui->annoLine->text(), ui->varLine->text(), ui->filterLine->text());
}

void SourcesDialog::on_cancelButton_clicked() {
    close();
}

void SourcesDialog::on_infoButton_clicked() {
    ui->chromosomeData->horizontalHeader()->hide();
    ui->chromosomeData->verticalHeader()->hide();
    ui->chromosomeData->setAlternatingRowColors(true);
    ui->chromosomeData->setRowCount(1);
    ui->chromosomeData->setColumnCount(4);
    ui->chromosomeData->setItem(0,0,new QTableWidgetItem("Chromosome"));
    ui->chromosomeData->setItem(0,1,new QTableWidgetItem("Reference"));
    ui->chromosomeData->setItem(0,2,new QTableWidgetItem("Annotations"));
    ui->chromosomeData->setItem(0,3,new QTableWidgetItem("Variants"));
    Sequence::info(this, ui->referenceLine->text());
    Annotation::info(this, ui->annoLine->text());
    Variation::info(this, ui->varLine->text());
}

void SourcesDialog::update(QString psFileType, QMap<QString,int>& paData) {
    QString lsChromosome;
    foreach (lsChromosome, paData.keys()) {
        update(psFileType, lsChromosome, paData[lsChromosome]);
    }
}

void SourcesDialog::update(QString psFileType, QString psChromosome, int piCount) {
    int i, liColumn = -1, liRow = -1;
    QTableWidgetItem *item;
    if ("sequence"   == psFileType) liColumn = 1;
    if ("annotation" == psFileType) liColumn = 2;
    if ("variation"  == psFileType) liColumn = 3;
    if (-1 == liColumn) return;
    if ("empty" == psChromosome) return;
    for (i = 1; i < ui->chromosomeData->rowCount(); i++) {
        item = ui->chromosomeData->item(i,0);
        if (item && item->text() == psChromosome) { liRow = i; break; }
    }
    if (-1 == liRow) {
        liRow = ui->chromosomeData->rowCount();
        ui->chromosomeData->insertRow(liRow);
        ui->chromosomeData->setItem(liRow,0,new QTableWidgetItem(psChromosome));
        ui->chromosomeData->setItem(liRow,1,new QTableWidgetItem("0"));
        ui->chromosomeData->setItem(liRow,2,new QTableWidgetItem("0"));
        ui->chromosomeData->setItem(liRow,3,new QTableWidgetItem("0"));
    }
    ui->chromosomeData->setItem(liRow,liColumn,new QTableWidgetItem(QString::number(piCount)));
}

