// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2024 by Agustin L. Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Entities.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Game
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Entities::Entities(Ref<Animator> Animator)
        : mAnimator { Animator }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    SPtr<Entity> Entities::Load(Ref<Reader> Reader)
    {
        const UInt32 ID   = Reader.ReadInt<UInt32>();
        const UInt32 Type = Reader.ReadInt<UInt32>();
        const UInt32 X    = Reader.ReadInt<UInt32>();
        const UInt32 Y    = Reader.ReadInt<UInt32>();

        SPtr<Entity> Actor = Create(ID, static_cast<Entity::Type>(Type), Vector2f(X, Y));
        if (Actor)
        {
            switch (Actor->GetType())
            {
            case Entity::Type::Object:
                OnDecode(Reader, CastPtr<Object>(Actor));
                break;
            case Entity::Type::Character:
                OnDecode(Reader, CastPtr<Character>(Actor));
                break;
            }
        }

        return Actor;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Entities::Save(Ref<Writer> Writer, ConstSPtr<Entity> Actor)
    {
        Writer.WriteInt<UInt32>(Actor->GetID());
        Writer.WriteInt<UInt32>(CastEnum(Actor->GetType()));

        Ref<const Vector2f> Position = Actor->GetPosition();
        Writer.WriteInt<UInt32>(Position.GetX());
        Writer.WriteInt<UInt32>(Position.GetY());

        switch (Actor->GetType())
        {
        case Entity::Type::Object:
            OnEncode(Writer, CastPtr<Object>(Actor));
            break;
        case Entity::Type::Character:
            OnEncode(Writer, CastPtr<Character>(Actor));
            break;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    SPtr<Entity> Entities::Create(UInt32 ID, Entity::Type Type, Ref<const Vector2f> Position)
    {
        SPtr<Entity> Actor = nullptr;

        switch (Type)
        {
        case Entity::Type::Object:
            Actor = NewPtr<Object>(ID, Position);
            break;
        case Entity::Type::Character:
            Actor = NewPtr<Character>(ID, Position);
            break;
        }

        if (Actor)
        {
            const auto Iterator = mDatabase.emplace(ID, Actor);

            if (Iterator.second)
            {
                mPartitioner.Insert(Actor);
            }
        }
        return Actor;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Entities::Update(ConstSPtr<Entity> Actor)
    {
        mPartitioner.Update(Actor);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Entities::Remove(ConstSPtr<Entity> Actor)
    {
        const Bool Succeed = mDatabase.erase(Actor->GetID());

        if (Succeed)
        {
            mPartitioner.Remove(Actor);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Entities::OnDecode(Ref<Reader> Reader, ConstSPtr<Object> Actor)
    {
        const UInt32 Animation = Reader.ReadInt<UInt32>();
        const UInt16 Rotation  = Reader.ReadInt<UInt16>();
        const UInt32 Tint      = Reader.ReadInt<UInt32>();

        Actor->SetDrawable(mAnimator.GetAnimation(Animation));
        Actor->SetRotation(Rotation);
        Actor->SetColor(Tint);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Entities::OnEncode(Ref<Writer> Writer, ConstSPtr<Object> Actor)
    {
        Ref<const Drawable> Drawable = Actor->GetDrawable();

        Writer.WriteInt<UInt32>(Drawable.HasAnimation() ? Drawable.GetAnimation()->ID : 0);
        Writer.WriteInt<UInt16>(Drawable.GetRotation());
        Writer.WriteInt<UInt32>(Drawable.GetColor().AsPacked());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Entities::OnDecode(Ref<Reader> Reader, ConstSPtr<Character> Actor)
    {
        // @TODO
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Entities::OnEncode(Ref<Writer> Writer, ConstSPtr<Character> Actor)
    {
        // @TODO
    }
}