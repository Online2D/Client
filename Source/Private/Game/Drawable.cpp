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

#include "Drawable.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Game
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Drawable::Drawable()
        : mRotation  { 0.0f },
          mColor     { 1.0f, 1.0f, 1.0f, 1.0f },
          mOrigin    { Pivot::TopLeft },
          mAnimation { nullptr }
    {
        SetState(State::Stopped);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ref<const Rectf> Drawable::GetFrame(Real64 Tick)
    {
        static constexpr Rectf kEmptyRect(0, 0, 0, 0);

        if (mAnimation == nullptr || mAnimation->Frames.empty() || mState == State::Stopped)
        {
            return kEmptyRect;
        }

        const UInt32 Count = mAnimation->Frames.size();
        UInt32 Offset = 0;

        if (Count > 1)
        {
            if (mTick == 0)
            {
                mTick = Tick;
            }

            Offset = static_cast<UInt32>((Tick - mTick) * (Count / mAnimation->Duration));

            if (Offset >= Count)
            {
                if (mState == State::Repeat)
                {
                    Offset %= Count;
                    mTick   = (Tick - Offset * mAnimation->Duration);
                }
                else
                {
                    Offset = Count - 1;
                    mState = State::Stopped;
                }
            }
        }
        return mAnimation->Frames[Offset];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Rectf Drawable::GetBoundaries(Pivot Origin, Ref<const Vector2f> Position, Ref<const Vector2f> Size)
    {
        static constexpr Rectf kMultiplier[] = {
            Rectf( 0.0f,  0.0f, 1.0f, 1.0f),  // TopLeft
            Rectf(-0.5f,  0.0f, 0.5f, 1.0f),  // TopCenter
            Rectf(-1.0f,  0.0f, 0.0f, 1.0f),  // TopRight
            Rectf( 0.0f, -0.5f, 1.0f, 0.5f),  // MiddleLeft
            Rectf(-0.5f, -0.5f, 0.5f, 0.5f),  // MiddleCenter
            Rectf(-1.0f, -0.5f, 0.0f, 0.5f),  // MiddleRight
            Rectf( 0.0f, -1.0f, 1.0f, 0.0f),  // BottomLeft
            Rectf(-0.5f, -1.0f, 0.5f, 0.0f),  // BottomCenter
            Rectf(-1.0f, -1.0f, 0.0f, 0.0f),  // BottomRight
        };
        return kMultiplier[CastEnum(Origin)] * Size + Position;
    }
}