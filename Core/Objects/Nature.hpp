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

#ifndef NATURE_HPP
#define NATURE_HPP

#include <QStringList>
#include <QTranslator>
#include <Core/Objects/Global.hpp>

namespace Nature
{
    QStringList getNatures();
    QString getNature(u8 nature);
    QStringList getFrameNatures();
    u8 getAdjustedNature(u8 nature);
    u8 getReversedNature(u8 nature);
};

#endif // NATURE_HPP