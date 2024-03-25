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

#include "Core/Core.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Game
{
    // -=(Undocumented)=-
    class Entity
    {
    public:

        // -=(Undocumented)=-
        enum class Type
        {
            // -=(Undocumented)=-
            Object,

            // -=(Undocumented)=-
            Character,
        };

        // -=(Undocumented)=-
        enum class Owner
        {
            // -=(Undocumented)=-
            Local,

            // -=(Undocumented)=-
            Remote,

            // -=(Undocumented)=-
            Mutual,
        };

    public:

        // -=(Undocumented)=-
        Entity(UInt32 ID, Type Type, Ref<const Vector3f> Position);

        // -=(Undocumented)=-
        virtual ~Entity() = default;

        // -=(Undocumented)=-
        UInt32 GetID() const
        {
            return mID;
        }

        // -=(Undocumented)=-
        Type GetType() const
        {
            return mType;
        }

        // -=(Undocumented)=-
        void SetPosition(Ref<const Vector3f> Position)
        {
            mPosition = Position;
        }

        // -=(Undocumented)=-
        Ref<const Vector3f> GetPosition() const
        {
            return mPosition;
        }

        // -=(Undocumented)=-
        void SetSize(Ref<const Vector2f> Size)
        {
            mSize = Size;
        }

        // -=(Undocumented)=-
        Ref<const Vector2f> GetSize() const
        {
            return mSize;
        }

        // -=(Undocumented)=-
        Rectf GetBoundaries() const
        {
            const Real32 HalfWidth = mSize.GetX() * 0.5f;
            return Rectf(
                mPosition.GetX() - HalfWidth,
                mPosition.GetY() - mSize.GetY(),
                mPosition.GetX() + HalfWidth,
                mPosition.GetY());
        }

    public:

        // -=(Undocumented)=-
        static constexpr UInt32 CalculateID(UInt32 Identifier, Owner Owner)
        {
            constexpr UInt32 OwnerBits      = 2;
            constexpr UInt32 OwnerMask      = GetMask<UInt32>(OwnerBits);
            constexpr UInt32 IdentifierBits = 30;
            constexpr UInt32 IdentifierMask = GetMask<UInt32>(IdentifierBits);

            return (CastEnum(Owner) & OwnerMask) << (IdentifierBits) | (Identifier & IdentifierMask);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UInt32   mID;
        Type     mType;
        Vector3f mPosition;
        Vector2f mSize;
    };
}