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
#include "Object.hpp"
#include "Character.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace World
{
    // -=(Undocumented)=-
    class Partitioner final
    {
    public:

        // -=(Undocumented)=-
        static constexpr UInt32 kDimension = 16;

    public:

        // -=(Undocumented)=-
        void Insert(ConstSPtr<Entity> Actor);

        // -=(Undocumented)=-
        void Update(ConstSPtr<Entity> Actor);

        // -=(Undocumented)=-
        void Remove(ConstSPtr<Entity> Actor);

        // -=(Undocumented)=-
        template<typename Function>
        void Query(Ref<const Rectf> Area, Function Executor) const
        {
            const Rectf Boundaries = Rectf::Max(Area, Rectf(0, 0, 0, 0)) / kDimension;

            for (UInt32 CellY = Boundaries.GetTop(); CellY <= Boundaries.GetBottom(); ++CellY)
            {
                for (UInt32 CellX = Boundaries.GetLeft(); CellX <= Boundaries.GetRight(); ++CellX)
                {
                    const auto Iterator = mCells.find(GetCell(CellX, CellY));
                    if (Iterator != mCells.end())
                    {
                        for (ConstSPtr<Entity> Actor : Iterator->second)
                        {
                            const Rectf Collision = Drawable::GetBoundaries(
                                Drawable::Pivot::BottomCenter, Actor->GetPosition(), Actor->GetSize());

                            if (Collision.Intersects(Area))
                            {
                                Executor(* Actor);
                            }
                        }
                    }
                }
            }
        }

    private:

        // -=(Undocumented)=-
        using Cell = Vector<SPtr<Entity>>;

        // -=(Undocumented)=-
        static constexpr UInt32 GetCell(UInt32 X, UInt32 Y)
        {
            return Y * UINT16_MAX + X;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

       Table<UInt32, Cell>  mCells;
       Table<UInt32, Rectf> mBoundaries;
    };
}