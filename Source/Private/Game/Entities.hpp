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
#include "Partitioner.hpp"
#include "Object.hpp"
#include "Character.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Game
{
    // -=(Undocumented)=-
    // @TODO: Entity Component (Not ECS shit) DOD system.
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
        SPtr<Entity> Create(UInt32 ID, Entity::Type Type, Ref<const Vector2f> Position);

        // -=(Undocumented)=-
        void Update(ConstSPtr<Entity> Actor);

        // -=(Undocumented)=-
        void Remove(ConstSPtr<Entity> Actor);

        // -=(Undocumented)=-
        SPtr<Entity> Find(UInt32 ID)
        {
            const auto Iterator = mDatabase.find(ID);
            return (Iterator != mDatabase.end() ? Iterator->second : nullptr);
        }

        // -=(Undocumented)=-
        template<typename Function>
        void Query(Ref<const Rectf> Area, Function Executor)
        {
            mPartitioner.template Query<Function>(Area, Executor);
        }

    private:

        // -=(Undocumented)=-
        void OnDecode(Ref<Reader> Reader, ConstSPtr<Object> Actor);

        // -=(Undocumented)=-
        void OnEncode(Ref<Writer> Writer, ConstSPtr<Object> Actor);

        // -=(Undocumented)=-
        void OnDecode(Ref<Reader> Reader, ConstSPtr<Character> Actor);

        // -=(Undocumented)=-
        void OnEncode(Ref<Writer> Writer, ConstSPtr<Character> Actor);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Partitioner                 mPartitioner;
        Ref<Animator>               mAnimator;
        Table<UInt32, SPtr<Entity>> mDatabase;
    };
}