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
    struct GatewayAccountData
        : public Network::Packet<GatewayAccountData, 4>
    {
        // -=(Undocumented)=-
        struct Entity
        {
            UInt ID;
            SStr Name;
            UInt Level;
            SStr Class;
            SStr Location;

            // -=(Undocumented)=-
            Entity() = default;

            // -=(Undocumented)=-
            Entity(UInt ID, CStr Name, UInt Level, CStr Class, CStr Location)
                : ID       { ID },
                  Name     { Name },
                  Level    { Level },
                  Class    { Class },
                  Location { Location }
            {
            }

            // -=(Undocumented)=-
            static Entity Read(Ref<Reader> Archive)
            {
                const UInt ID       = Archive.ReadInt<UInt>();
                const CStr Name     = Archive.ReadString8();
                const UInt Level    = Archive.ReadInt<UInt>();
                const CStr Class    = Archive.ReadString8();
                const CStr Location = Archive.ReadString8();
                return Entity(ID, Name, Level, Class, Location);
            }

            // -=(Undocumented)=-
            static void Write(Ref<Writer> Archive, Ref<const Entity> Entry)
            {
                Archive.WriteInt(Entry.ID);
                Archive.WriteString8(Entry.Name);
                Archive.WriteInt(Entry.Level);
                Archive.WriteString8(Entry.Class);
                Archive.WriteString8(Entry.Location);
            }
        };

        Vector<Entity> Entities;

        // -=(Undocumented)=-
        GatewayAccountData() = default;

        // -=(Undocumented)=-
        GatewayAccountData(Ref<Reader> Archive)
        {
            OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        void Add(Ref<const Entity> Entity)
        {
            Entities.emplace_back(Entity);
        }

        // -=(Undocumented)=-
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
            Archive.SerializeList(Entities);
        }
    };
}