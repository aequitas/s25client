// Copyright (c) 2005 - 2017 Settlers Freaks (sf-team at siedler25.org)
//
// This file is part of Return To The Roots.
//
// Return To The Roots is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Return To The Roots is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Return To The Roots. If not, see <http://www.gnu.org/licenses/>.

#include "Savegame.h"
#include "s25util/BinaryFile.h"

std::string Savegame::GetSignature() const
{
    return "RTTRSV";
}

uint16_t Savegame::GetVersion() const
{
    // Note: If you increase the version, reset currentGameDataVersion in SerializedGameData.cpp (see note there)
    // Note2: Also remove the workaround for the team in BasePlayerInfo
    return 4; // SaveGameVersion -- Updater signature, do NOT remove
}

//////////////////////////////////////////////////////////////////////////

Savegame::Savegame() : start_gf(0) {}

Savegame::~Savegame() = default;

bool Savegame::Save(const boost::filesystem::path& filepath, const std::string& mapName)
{
    BinaryFile file;

    return file.Open(filepath, OFM_WRITE) && Save(file, mapName);
}

bool Savegame::Save(BinaryFile& file, const std::string& mapName)
{
    WriteAllHeaderData(file, mapName);
    WritePlayerData(file);
    WriteGGS(file);
    WriteGameData(file);

    return true;
}

bool Savegame::Load(const boost::filesystem::path& filePath, const SaveGameDataToLoad what)
{
    BinaryFile file;

    return file.Open(filePath, OFM_READ) && Load(file, what);
}

bool Savegame::Load(BinaryFile& file, const SaveGameDataToLoad what)
{
    try
    {
        ClearPlayers();
        sgd.Clear();
        if(!ReadAllHeaderData(file))
            return false;

        if(what == SaveGameDataToLoad::Header)
            return true;

        ReadPlayerData(file);
        ReadGGS(file);

        if(what == SaveGameDataToLoad::HeaderAndSettings)
            return true;

        ReadGameData(file);
    } catch(std::runtime_error& e)
    {
        lastErrorMsg = e.what();
        return false;
    }

    return true;
}

void Savegame::WriteExtHeader(BinaryFile& file, const std::string& mapName)
{
    SavedFile::WriteExtHeader(file, mapName);
    file.WriteUnsignedInt(start_gf);
}

bool Savegame::ReadExtHeader(BinaryFile& file)
{
    if(!SavedFile::ReadExtHeader(file))
        return false;
    start_gf = file.ReadUnsignedInt();
    return true;
}

void Savegame::WriteGameData(BinaryFile& file)
{
    sgd.WriteToFile(file);
}

bool Savegame::ReadGameData(BinaryFile& file)
{
    sgd.ReadFromFile(file);
    return true;
}
