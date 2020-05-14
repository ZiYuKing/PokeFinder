/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "ProfileCalibrator5.hpp"
#include "ui_ProfileCalibrator5.h"
#include <Core/Enum/DSType.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Language.hpp>
#include <Core/Gen5/Searchers/ProfileSearcher5.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Util/IVCalculator.hpp>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

ProfileCalibrator5::ProfileCalibrator5(QWidget *parent) : QWidget(parent), ui(new Ui::ProfileCalibrator5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
    updateParameters();
}

ProfileCalibrator5::~ProfileCalibrator5()
{
    QSettings setting;
    setting.beginGroup("profileCalibrator5");
    setting.setValue("geometry", this->saveGeometry());

    delete ui;
}

void ProfileCalibrator5::setupModels()
{
    model = new QStandardItemModel(ui->tableView);
    model->setHorizontalHeaderLabels({ tr("Seconds"), tr("VCount"), tr("Timer0"), tr("GxStat"), tr("VFrame"), tr("Seed") });
    ui->tableView->setModel(model);

    menu = new QMenu(ui->tableView);

    ui->textBoxMinVCount->setValues(0, 0xFF, 2, 16);
    ui->textBoxMaxVCount->setValues(0, 0xFF, 2, 16);
    ui->textBoxMinTimer0->setValues(InputType::Seed16Bit);
    ui->textBoxMaxTimer0->setValues(InputType::Seed16Bit);
    ui->textBoxMinGxStat->setValues(0, 99, 2, 16);
    ui->textBoxMaxGxStat->setValues(0, 99, 2, 16);
    ui->textBoxMinVFrame->setValues(0, 99, 2, 16);
    ui->textBoxMaxVFrame->setValues(0, 99, 2, 16);
    ui->textBoxMACAddress->setValues(0, 0xFFFFFFFFFF, 10, 16);

    ui->comboBoxVersion->setup({ Game::Black, Game::White, Game::Black2, Game::White2 });
    ui->comboBoxLanguage->setup({ Language::English, Language::Spanish, Language::French, Language::Italian, Language::German,
                                  Language::Japanese, Language::Korean });
    ui->comboBoxDSType->setup({ DSType::DSOriginal, DSType::DSi, DSType::DS3 });

    ui->comboBoxKeypress1->addItem(tr("None"), 0);
    ui->comboBoxKeypress2->addItem(tr("None"), 0);
    ui->comboBoxKeypress3->addItem(tr("None"), 0);
    for (int i = 0; i < 12; i++)
    {
        ui->comboBoxKeypress1->addItem(Translator::getKeypress(i), 1 << i);
        ui->comboBoxKeypress2->addItem(Translator::getKeypress(i), 1 << i);
        ui->comboBoxKeypress3->addItem(Translator::getKeypress(i), 1 << i);
    }

    QAction *createProfile = menu->addAction("Create profile");
    connect(createProfile, &QAction::triggered, this, &ProfileCalibrator5::createProfile);

    connect(ui->pushButtonIVCalculator, &QPushButton::clicked, this, &ProfileCalibrator5::openIVCalculator);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &ProfileCalibrator5::search);
    connect(ui->pushButtonClearTable, &QPushButton::clicked, this, &ProfileCalibrator5::clearTable);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &ProfileCalibrator5::tableViewContextMenu);
    connect(ui->comboBoxVersion, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ProfileCalibrator5::versionIndexChanged);
    connect(ui->comboBoxDSType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ProfileCalibrator5::dsTypeIndexChanged);

    QSettings setting;
    setting.beginGroup("profileCalibrator5");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void ProfileCalibrator5::updateParameters()
{
    Game version = static_cast<Game>(ui->comboBoxVersion->getCurrentInt());
    DSType dsType = static_cast<DSType>(ui->comboBoxDSType->getCurrentInt());

    switch (dsType)
    {
    case DSType::DSOriginal:
        if (version & Game::BW)
        {
            ui->textBoxMinVCount->setText("50");
            ui->textBoxMaxVCount->setText("70");
            ui->textBoxMinTimer0->setText("C60");
            ui->textBoxMaxTimer0->setText("CA0");
        }
        else
        {
            ui->textBoxMinVCount->setText("70");
            ui->textBoxMaxVCount->setText("90");
            ui->textBoxMinTimer0->setText("10E0");
            ui->textBoxMaxTimer0->setText("1130");
        }
        break;
    case DSType::DSi:
    case DSType::DS3:
        if (version & Game::BW)
        {
            ui->textBoxMinVCount->setText("80");
            ui->textBoxMaxVCount->setText("92");
            ui->textBoxMinTimer0->setText("1140");
            ui->textBoxMaxTimer0->setText("12D0");
        }
        else
        {
            ui->textBoxMinVCount->setText("A0");
            ui->textBoxMaxVCount->setText("C0");
            ui->textBoxMinTimer0->setText("1600");
            ui->textBoxMaxTimer0->setText("1790");
        }
        break;
    }

    ui->textBoxMinGxStat->setText("6");
    ui->textBoxMaxGxStat->setText("6");
    ui->textBoxMinVFrame->setText("0");
    ui->textBoxMaxVFrame->setText("10");
}

void ProfileCalibrator5::updateProgress(const QVector<QList<QStandardItem *>> &frames, int progress)
{
    for (auto &frame : frames)
    {
        model->appendRow(frame);
    }
    ui->progressBar->setValue(progress);
}

void ProfileCalibrator5::openIVCalculator()
{
    auto *iv = new IVCalculator();
    iv->show();
    iv->raise();
}

void ProfileCalibrator5::clearTable()
{
    model->removeRows(0, model->rowCount());
}

void ProfileCalibrator5::search()
{
    QVector<u8> minIVs = { static_cast<u8>(ui->spinBoxMinHP->value()),  static_cast<u8>(ui->spinBoxMinAtk->value()),
                           static_cast<u8>(ui->spinBoxMinDef->value()), static_cast<u8>(ui->spinBoxMinSpA->value()),
                           static_cast<u8>(ui->spinBoxMinSpD->value()), static_cast<u8>(ui->spinBoxMinSpe->value()) };
    QVector<u8> maxIVs = { static_cast<u8>(ui->spinBoxMaxHP->value()),  static_cast<u8>(ui->spinBoxMaxAtk->value()),
                           static_cast<u8>(ui->spinBoxMaxDef->value()), static_cast<u8>(ui->spinBoxMaxSpA->value()),
                           static_cast<u8>(ui->spinBoxMaxSpD->value()), static_cast<u8>(ui->spinBoxMaxSpe->value()) };

    QDate date = ui->dateTimeEdit->date();
    QTime time = ui->dateTimeEdit->time();
    int minSeconds = ui->spinBoxMinSeconds->value();
    int maxSeconds = ui->spinBoxMaxSeconds->value();
    u8 minVCount = ui->textBoxMinVCount->getUChar();
    u8 maxVCount = ui->textBoxMaxVCount->getUChar();
    u16 minTimer0 = ui->textBoxMinTimer0->getUShort();
    u16 maxTimer0 = ui->textBoxMaxTimer0->getUShort();
    u8 minGxStat = ui->textBoxMinGxStat->getUChar();
    u8 maxGxStat = ui->textBoxMaxGxStat->getUChar();
    u8 minVFrame = ui->textBoxMinVFrame->getUChar();
    u8 maxVFrame = ui->textBoxMaxVFrame->getUChar();
    bool softReset = ui->checkBoxSoftReset->isChecked();
    Game version = static_cast<Game>(ui->comboBoxVersion->getCurrentInt());
    Language language = static_cast<Language>(ui->comboBoxLanguage->getCurrentInt());
    DSType dsType = static_cast<DSType>(ui->comboBoxDSType->getCurrentInt());
    u64 mac = ui->textBoxMACAddress->getULong();
    u16 keypress
        = ui->comboBoxKeypress1->getCurrentUShort() | ui->comboBoxKeypress2->getCurrentUShort() | ui->comboBoxKeypress3->getCurrentUShort();

    if (minSeconds > maxSeconds || minVCount > maxVCount || minTimer0 > maxTimer0 || minGxStat > maxGxStat || minVFrame > maxVFrame)
    {
        QMessageBox error;
        error.setText(tr("Some min values are greater then max values"));
        error.exec();
        return;
    }

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    auto *searcher = new ProfileSearcher5(minIVs, maxIVs, date, time, minSeconds, maxSeconds, minVCount, maxVCount, minTimer0, maxTimer0,
                                          minGxStat, maxGxStat, softReset, version, language, dsType, mac, keypress);

    int maxProgress = (maxSeconds - minSeconds + 1) * (maxVCount - minVCount + 1) * (maxTimer0 - minTimer0 + 1)
        * (maxGxStat - maxGxStat + 1) * (maxVFrame - minVFrame + 1);
    ui->progressBar->setRange(0, maxProgress);

    QSettings settings;
    int threads = settings.value("settings/threads", QThread::idealThreadCount()).toInt();

    auto *thread = QThread::create([=] { searcher->startSearch(threads, minVFrame, maxVFrame); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, [=] { updateProgress(searcher->getResults(), searcher->getProgress()); });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        updateProgress(searcher->getResults(), searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void ProfileCalibrator5::createProfile()
{
    int row = ui->tableView->currentIndex().row();
    if (row < 0)
    {
        QMessageBox error;
        error.setText(tr("Please select a entry."));
        error.exec();
        return;
    }

    // TODO
}

void ProfileCalibrator5::tableViewContextMenu(QPoint pos)
{
    if (model->rowCount() > 0)
    {
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void ProfileCalibrator5::versionIndexChanged(int index)
{
    if (index >= 0)
    {
        updateParameters();
    }
}

void ProfileCalibrator5::dsTypeIndexChanged(int index)
{
    if (index >= 0)
    {
        updateParameters();
    }
}
