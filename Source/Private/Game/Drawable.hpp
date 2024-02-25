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

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Game
{
    // -=(Undocumented)=-
    class Drawable final
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
            : mColor     { 1.0f, 1.0f, 1.0f, 1.0f },
              mAnimation { nullptr }
        {
            SetState(State::Stopped);
        }

        // -=(Undocumented)=-
        void SetPosition(Vector3f Position)
        {
            mPosition = Position;
        }

        // -=(Undocumented)=-
        Ref<const Vector3f> GetPosition() const
        {
            return mPosition;
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
        Ptr<const Animation> GetAnimation() const
        {
            return mAnimation;
        }

        // -=(Undocumented)=-
        Ref<const Rectf> GetFrame(Real64 Tick);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector3f             mPosition;
        Color                mColor;
        State                mState;
        Ptr<const Animation> mAnimation;
        Real64               mTick;
    };
}