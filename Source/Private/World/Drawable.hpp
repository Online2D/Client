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

#include "Animation.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace World
{
    // -=(Undocumented)=-
    class Drawable final
    {
    public:

        // -=(Undocumented)=-
        enum class State
        {
            // -=(Undocumented)=-
            Started,

            // -=(Undocumented)=-
            Stopped,

            // -=(Undocumented)=-
            Repeat,
        };

        // -=(Undocumented)=-
        enum class Pivot
        {
            // -=(Undocumented)=-
            TopLeft,

            // -=(Undocumented)=-
            TopCenter,

            // -=(Undocumented)=-
            TopRight,

            // -=(Undocumented)=-
            MiddleLeft,

            // -=(Undocumented)=-
            MiddleCenter,

            // -=(Undocumented)=-
            MiddleRight,

            // -=(Undocumented)=-
            BottomLeft,

            // -=(Undocumented)=-
            BottomCenter,

            // -=(Undocumented)=-
            BottomRight,
        };

        // -=(Undocumented)=-
        enum class Order
        {
            // -=(Undocumented)=-
            Background,

            // -=(Undocumented)=-
            Middle,

            // -=(Undocumented)=-
            Foreground,
        };

    public:

        // -=(Undocumented)=-
        Drawable();

        // -=(Undocumented)=-
        void SetRotation(Real32 Angles)
        {
            mRotation = Angles;
        }

        // -=(Undocumented)=-
        Real32 GetRotation() const
        {
            return mRotation;
        }

        // -=(Undocumented)=-
        void SetColor(Color Tint)
        {
            mColor = Tint;
        }

        // -=(Undocumented)=-
        Ref<const Color> GetColor() const
        {
            return mColor;
        }

        // -=(Undocumented)=-
        void SetOrigin(Pivot Origin)
        {
            mOrigin = Origin;
        }

        // -=(Undocumented)=-
        Pivot GetOrigin() const
        {
            return mOrigin;
        }

        // -=(Undocumented)=-
        void SetState(State State)
        {
            mState = State;

            if (State == State::Stopped)
            {
                mTick = 0;
            }
        }

        // -=(Undocumented)=-
        State GetState() const
        {
            return mState;
        }

        // -=(Undocumented)=-
        void SetAnimation(Ptr<const Animation> Animation)
        {
            mAnimation = Animation;
        }

        // -=(Undocumented)=-
        Bool HasAnimation() const
        {
            return mAnimation != nullptr;
        }

        // -=(Undocumented)=-
        Ptr<const Animation> GetAnimation() const
        {
            return mAnimation;
        }

        // -=(Undocumented)=-
        Ref<const Rectf> GetFrame(Real64 Tick);

        // -=(Undocumented)=-
        Rectf GetBoundaries(Ref<const Vector2f> Position) const
        {
            Rectf Boundaries(0.0f, 0.0f, 0.0f, 0.0f);

            if (mAnimation)
            {
                Boundaries = GetBoundaries(mOrigin, Position, Vector2f(mAnimation->Width, mAnimation->Height));
            }
            return Boundaries;
        }

    public:

        // -=(Undocumented)=-
        static Rectf GetBoundaries(Pivot Origin, Ref<const Vector2f> Position, Ref<const Vector2f> Size);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Real32               mRotation;
        Color                mColor;
        Pivot                mOrigin;
        State                mState;
        Ptr<const Animation> mAnimation;
        Real64               mTick;
    };
}