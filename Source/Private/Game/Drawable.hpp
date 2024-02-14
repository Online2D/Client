// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2024 by Online-MMO-Engine Team. All rights reserved.
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
#include <Core/Math/Color.hpp>
#include <Core/Math/Vector3.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Game
{
    // -=(Undocumented)=-
    class Drawable
    {
    public:

        // -=(Undocumented)=-
        enum class State
        {
            Started,
            Stopped,
            Repeat,
        };

    public:

        // -=(Undocumented)=-
        Drawable()
        {
            SetState(State::Stopped);
        }

        // -=(Undocumented)=-
        void SetPosition(Vector3f Position)
        {
            mPosition = Position;
        }

        // -=(Undocumented)=-
        Vector3f GetPosition() const
        {
            return mPosition;
        }

        // -=(Undocumented)=-
        void SetColor(Color Tint)
        {
            mColor = Tint;
        }

        // -=(Undocumented)=-
        Color GetColor() const
        {
            return mColor;
        }

        // -=(Undocumented)=-
        void SetState(State State)
        {
            if (State == State::Stopped)
            {
                mAnimationTime = 0;
            }

            mState = State;
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
        Ptr<const Animation> GetAnimation() const
        {
            return mAnimation;
        }

        // -=(Undocumented)=-
        Ptr<const Animation::Frame> GetFrame(Real64 Time);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector3f             mPosition;
        Color                mColor;
        State                mState;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<const Animation> mAnimation;
        Real64               mAnimationTime;
    };
}