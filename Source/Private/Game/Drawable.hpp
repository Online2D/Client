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
            : mState         { State::Stopped },
              mAnimationTime { 0 }
        {
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
        void SetTint(Color Tint)
        {
            mTint = Tint;
        }

        // -=(Undocumented)=-
        Color GetTint() const
        {
            return mTint;
        }

        // -=(Undocumented)=-
        void SetState(State State)
        {
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
        Ptr<const Animation::Frame> GetFrame(Real64 Time)
        {
            if (mAnimation == nullptr || mState == State::Stopped)
            {
                return nullptr;
            }

            if (mAnimationTime == 0)
            {
                mAnimationTime = Time;
            }

            UInt Difference = static_cast<UInt>((Time - mAnimationTime) * 1000.0f);
            UInt Offset     = 0;

            if (Difference >= mAnimation->Duration)
            {
                if (mState == State::Repeat)
                {
                    Offset         = (Offset % mAnimation->Duration) % mAnimation->Frames.size() - 1;
                    mAnimationTime = Time;
                }
                else
                {
                    Offset         = mAnimation->Frames.size() - 1;
                    mState         = State::Stopped;
                    mAnimationTime = 0;
                }
            }
            return & mAnimation->Frames[Offset];
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector3f             mPosition;
        Color                mTint;
        State                mState;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<const Animation> mAnimation;
        Real64               mAnimationTime;
    };
}