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

namespace Gameplay
{
    // -=(Undocumented)=-
    struct GatewayAccountError
        : public Network::Packet<GatewayAccountError, 0>
    {
        // -=(Undocumented)=-
        enum class ID
        {
            Unknown,
            Mismatch,
            Exist,
            Online,
        };

        ID Type;

        // -=(Undocumented)=-
        GatewayAccountError() = default;

        // -=(Undocumented)=-
        GatewayAccountError(Ref<Reader> Archive)
        {
            OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        GatewayAccountError(ID Type)
            : Type { Type }
        {
        }

        // -=(Undocumented)=-
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
            Archive.SerializeEnum(Type);
        }
    };

    // -=(Undocumented)=-
    struct GatewayAccountLogin
        : public Network::Packet<GatewayAccountLogin, 1>
    {
        SStr Username;
        SStr Password;

        // -=(Undocumented)=-
        GatewayAccountLogin() = default;

        // -=(Undocumented)=-
        GatewayAccountLogin(Ref<Reader> Archive)
        {
            OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        GatewayAccountLogin(CStr Username, CStr Password)
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
    struct GatewayAccountRegister
        : public Network::Packet<GatewayAccountRegister, 2>
    {
        SStr Username;
        SStr Password;
        SStr Email;

        // -=(Undocumented)=-
        GatewayAccountRegister() = default;

        // -=(Undocumented)=-
        GatewayAccountRegister(Ref<Reader> Archive)
        {
            OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        GatewayAccountRegister(CStr Username, CStr Password, CStr Email)
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
    struct GatewayAccountDelete
        : public Network::Packet<GatewayAccountDelete, 3>
    {
        SStr Username;
        SStr Password;

        // -=(Undocumented)=-
        GatewayAccountDelete() = default;

        // -=(Undocumented)=-
        GatewayAccountDelete(Ref<Reader> Archive)
        {
            OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        GatewayAccountDelete(CStr Username, CStr Password)
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
    struct GatewayAccountList
        : public Network::Packet<GatewayAccountList, 4>
    {
        // @TODO
    };
}