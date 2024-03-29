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
          mPivot     { Pivot::TopLeft },
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

    Rectf Drawable::GetBoundaries(Ref<const Vector2f> Position) const
    {
        Rectf Boundaries;

        if (mAnimation)
        {
            const Real32 X      = Position.GetX();
            const Real32 Y      = Position.GetY();
            const Real32 Width  = mAnimation->Width;
            const Real32 Height = mAnimation->Height;

            switch (mPivot)
            {
            case Pivot::TopLeft:
                Boundaries = Rectf(X, Y, X + Width, Y + Height);
                break;
            case Pivot::TopCenter:
                Boundaries = Rectf(X - Width * 0.5f, Y, X + Width * 0.5f, Y + Height);
                break;
            case Pivot::TopRight:
                Boundaries = Rectf(X - Width, Y, X, Y + Height);
                break;
            case Pivot::MiddleLeft:
                Boundaries = Rectf(X, Y - Height * 0.5f, X + Width, Y + Height * 0.5f);
                break;
            case Pivot::MiddleCenter:
                Boundaries = Rectf(X - Width * 0.5f, Y - Height * 0.5f, X + Width * 0.5f, Y + Height * 0.5f);
                break;
            case Pivot::MiddleRight:
                Boundaries = Rectf(X - Width, Y - Height * 0.5f, X, Y + Height * 0.5f);
                break;
            case Pivot::BottomLeft:
                Boundaries = Rectf(X, Y - Height, X + Width, Y);
                break;
            case Pivot::BottomCenter:
                Boundaries = Rectf(X - Width * 0.5f, Y - Height, X + Width * 0.5f, Y);
                break;
            case Pivot::BottomRight:
                Boundaries = Rectf(X - Width, Y - Height, X, Y);
                break;
            }
        }
        return Boundaries;
    }
}