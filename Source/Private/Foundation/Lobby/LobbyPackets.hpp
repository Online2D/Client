// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2024 by Agustin L. Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include <Network/Packet.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Foundation
{
    // -=(Undocumented)=-
    struct LobbyCharacterEnter
        : public Network::Packet<LobbyCharacterEnter, 10>
    {
        UInt Character;

        // -=(Undocumented)=-
        LobbyCharacterEnter() = default;

        // -=(Undocumented)=-
        LobbyCharacterEnter(Ref<Reader> Archive)
        {
            OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        LobbyCharacterEnter(UInt Character)
            : Character { Character }
        {
        }

        // -=(Undocumented)=-
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
            Archive.SerializeInt(Character);
        }
    };

    // -=(Undocumented)=-
    struct LobbyCharacterDelete
        : public Network::Packet<LobbyCharacterDelete, 11>
    {
        UInt Character;

        // -=(Undocumented)=-
        LobbyCharacterDelete() = default;

        // -=(Undocumented)=-
        LobbyCharacterDelete(Ref<Reader> Archive)
        {
            OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        LobbyCharacterDelete(UInt Character)
            : Character { Character }
        {
        }

        // -=(Undocumented)=-
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
            Archive.SerializeInt(Character);
        }
    };

    // -=(Undocumented)=-
    struct LobbyCharacterCreate
        : public Network::Packet<LobbyCharacterCreate, 12>
    {
        // TODO

        // -=(Undocumented)=-
        LobbyCharacterCreate() = default;

        // -=(Undocumented)=-
        LobbyCharacterCreate(Ref<Reader> Archive)
        {
            OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
        }
    };
}