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
        const UInt32 X    = Reader.ReadReal32();
        const UInt32 Y    = Reader.ReadReal32();
        const UInt32 Z    = Reader.ReadReal32();

        SPtr<Entity> Actor = Create(ID, static_cast<Entity::Type>(Type), Vector3f(X, Y, Z));
        if (Actor)
        {
            switch (Actor->GetArchetype())
            {
            case Entity::Type::Object:
                OnDecode(Reader, CastPtr<Object>(Actor));
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
        Writer.WriteInt<UInt32>(CastEnum(Actor->GetArchetype()));

        Ref<const Vector3f> Position = Actor->GetPosition();
        Writer.WriteReal32(Position.GetX());
        Writer.WriteReal32(Position.GetY());
        Writer.WriteReal32(Position.GetZ());

        switch (Actor->GetArchetype())
        {
        case Entity::Type::Object:
            OnEncode(Writer, CastPtr<Object>(Actor));
            break;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    SPtr<Entity> Entities::Create(UInt32 ID, Entity::Type Type, Ref<const Vector3f> Position)
    {
        SPtr<Entity> Entity = nullptr;

        switch (Type)
        {
        case Entity::Type::Object:
            Entity = NewPtr<Object>(ID, Position);
            break;
        }

        if (Entity)
        {
            const auto Iterator = mDatabase.emplace(ID, Entity);

            if (Iterator.second)
            {
                // TODO: Insert to Tree
            }
        }
        return Entity;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Entities::Remove(ConstSPtr<Entity> Actor)
    {
        const Bool Succeed = mDatabase.erase(Actor->GetID());

        if (Succeed)
        {
            // TODO: Remove from Tree
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Entities::Query(Ref<const Rectf> Area, FPtr<void (Ref<Entity>)> Executor)
    {
        for (const auto Iterator : mDatabase)   // @TODO: Replace with data partition structure
        {
            ConstSPtr<Entity> Entity = Iterator.second;

            if (Entity->GetBoundaries().Intersects(Area))
            {
                Executor(* Entity);
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Entities::OnDecode(Ref<Reader> Reader, ConstSPtr<Object> Entity)
    {
        const UInt32 Animation = Reader.ReadInt<UInt32>();
        const UInt16 Rotation  = Reader.ReadInt<UInt16>();
        const UInt32 Tint      = Reader.ReadInt<UInt32>();

        Entity->SetDrawable(mAnimator.GetAnimation(Animation));
        Entity->SetRotation(Rotation);
        Entity->SetColor(Tint);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Entities::OnEncode(Ref<Writer> Writer, ConstSPtr<Object> Entity)
    {
        Ref<const Drawable> Drawable = Entity->GetDrawable();

        Writer.WriteInt<UInt32>(Drawable.HasAnimation() ? Drawable.GetAnimation()->ID : 0);
        Writer.WriteInt<UInt16>(Drawable.GetRotation());
        Writer.WriteInt<UInt32>(Drawable.GetColor().AsPacked());
    }
}