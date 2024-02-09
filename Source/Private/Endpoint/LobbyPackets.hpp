// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2024 by Online-MMO-Engine Team. All rights reserved.
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

namespace Endpoint
{
    // -=(Undocumented)=-
    struct LobbyAccountLogin
        : public Network::Packet<LobbyAccountLogin, 1>
    {
        SStr Username;
        SStr Password;

        // -=(Undocumented)=-
        LobbyAccountLogin() = default;

        // -=(Undocumented)=-
        LobbyAccountLogin(CStr Username, CStr Password)
            : Username { Username },
              Password { Password }
        {
        }

        // -=(Undocumented)=-
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
            Archive.SerializeString8(Username);
            Archive.SerializeString8(Password);
        }
    };

    // -=(Undocumented)=-
    struct LobbyAccountRegister
        : public Network::Packet<LobbyAccountRegister, 2>
    {
        SStr Username;
        SStr Password;
        SStr Email;

        // -=(Undocumented)=-
        LobbyAccountRegister() = default;

        // -=(Undocumented)=-
        LobbyAccountRegister(CStr Username, CStr Password, CStr Email)
            : Username { Username },
              Password { Password },
              Email    { Email }
        {
        }

        // -=(Undocumented)=-
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
            Archive.SerializeString8(Username);
            Archive.SerializeString8(Password);
            Archive.SerializeString8(Email);
        }
    };

    // -=(Undocumented)=-
    struct LobbyAccountDelete
        : public Network::Packet<LobbyAccountDelete, 3>
    {
        SStr Username;

        // -=(Undocumented)=-
        LobbyAccountDelete() = default;

        // -=(Undocumented)=-
        LobbyAccountDelete(CStr Username)
            : Username{ Username }
        {
        }

        // -=(Undocumented)=-
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
            Archive.SerializeString8(Username);
        }
    };
}