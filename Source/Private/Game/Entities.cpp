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
        SPtr<Entity> Entity = nullptr;

        switch (Type)
        {
        case Entity::Type::Object:
            Entity = NewPtr<Object>(ID, Position);
            break;
        case Entity::Type::Character:
            Entity = NewPtr<Character>(ID, Position);
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

            const Rectf Boundaries = Drawable::GetBoundaries(
                Game::Drawable::Pivot::BottomCenter, Entity->GetPosition(), Entity->GetSize());
            
            if (Boundaries.Intersects(Area))
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

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Entities::OnDecode(Ref<Reader> Reader, ConstSPtr<Character> Character)
    {
        // @TODO
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Entities::OnEncode(Ref<Writer> Writer, ConstSPtr<Character> Character)
    {
        // @TODO
    }

}