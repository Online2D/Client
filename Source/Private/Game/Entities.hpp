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

#include "Animator.hpp"
#include "Entity.hpp"
#include "Object.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Game
{
    // -=(Undocumented)=-
    // @TODO: Entity Component (Not ECS shit) DOD system.
    // @TODO: Loose/Tight Hash Grid for culling and collision.
    class Entities final
    {
    public:

        // -=(Undocumented)=-
        Entities(Ref<Animator> Animator);

        // -=(Undocumented)=-
        SPtr<Entity> Load(Ref<Reader> Reader);

        // -=(Undocumented)=-
        void Save(Ref<Writer> Writer, ConstSPtr<Entity> Actor);

        // -=(Undocumented)=-
        SPtr<Entity> Create(UInt32 ID, Entity::Type Archetype, Ref<const Vector3f> Position);

        // -=(Undocumented)=-
        void Remove(ConstSPtr<Entity> Actor);

        // -=(Undocumented)=-
        SPtr<Entity> Find(UInt32 ID)
        {
            const auto Iterator = mDatabase.find(ID);
            return (Iterator != mDatabase.end() ? Iterator->second : nullptr);
        }

        // -=(Undocumented)=-
        void Query(Ref<const Rectf> Area, FPtr<void (Ref<Entity>)> Executor);

    private:

        // -=(Undocumented)=-
        void OnDecode(Ref<Reader> Reader, ConstSPtr<Object> Entity);

        // -=(Undocumented)=-
        void OnEncode(Ref<Writer> Writer, ConstSPtr<Object> Entity);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ref<Animator>               mAnimator;
        Table<UInt32, SPtr<Entity>> mDatabase;
    };
}