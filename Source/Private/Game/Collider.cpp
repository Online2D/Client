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

#include "Collider.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Game
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Collider::Collides(Ref<const Collider> Instigator) const
    {
        // Calculate Minkowski Difference
        Stack<Vector2f, kMaxPoints * kMaxPoints> MinkowskiDifference;
        for (Ref<const Vector2f> Point1 : mPoints)
        {
            for (Ref<const Vector2f> Point2 : Instigator.mPoints)
            {
                MinkowskiDifference.push_back(Point1 - Point2);
            }
        }

        // Check if origin is inside Minkowski Difference
        for (UInt32 Element = 0; Element < MinkowskiDifference.size(); ++Element)
        {
            Ref<const Vector2f> P0 = MinkowskiDifference[Element];
            Ref<const Vector2f> P1 = MinkowskiDifference[(Element + 1) % MinkowskiDifference.size()];

            // Check if the origin lies between two consecutive points
            if ((P0.GetY() <= 0 && P1.GetY() > 0) || (P1.GetY() <= 0 && P0.GetY() > 0))
            {
                const Real32 Intersection = P0.GetX() - P0.GetY() * (P1.GetX() - P0.GetX()) / (P1.GetY() - P0.GetY());
                if (Intersection > 0)
                {
                    return true;
                }
            }
        }
        return false;
    }
}