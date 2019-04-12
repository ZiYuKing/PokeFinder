/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef PROFILEEDITOR5_HPP
#define PROFILEEDITOR5_HPP

#include <QDialog>
#include <QMessageBox>
#include <Core/Gen5/Profile5.hpp>
#include <Forms/Gen5/ProfileCalibrator5.hpp>

namespace Ui
{
    class ProfileEditor5;
}

class ProfileEditor5 : public QDialog
{
    Q_OBJECT

signals:
    void newProfile(Profile5);
    void editProfile(Profile5, Profile5);

public:
    explicit ProfileEditor5(QWidget *parent = nullptr);
    explicit ProfileEditor5(const Profile5 &profile, QWidget *parent = nullptr);
    ~ProfileEditor5() override;
    Profile5 getNewProfile();
    Profile5 getOriginal();

private:
    Ui::ProfileEditor5 *ui;
    bool isEditing = false;
    Profile5 original;
    Profile5 fresh;

    void setupModels();

private slots:
    void on_pushButtonAccept_clicked();
    void on_pushButtonFindParameters_clicked();

};

#endif // PROFILEEDITOR5_HPP
