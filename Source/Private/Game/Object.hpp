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

#include "Entity.hpp"
#include "Drawable.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Game
{
    // -=(Undocumented)=-
    class Object : public Entity
    {
    public:

        // -=(Undocumented)=-
        Object(UInt32 ID, Ref<const Vector2f> Position);

        // -=(Undocumented)=-
        void SetColor(Ref<const Color> Tint)
        {
            mDrawable.SetColor(Tint);
        }

        // -=(Undocumented)=-
        Ref<const Color> GetColor() const
        {
            return mDrawable.GetColor();
        }

        // -=(Undocumented)=-
        void SetRotation(Real32 Rotation)
        {
            mDrawable.SetRotation(Rotation);
        }

        // -=(Undocumented)=-
        Real32 GetRotation() const
        {
            return mDrawable.GetRotation();
        }

        // -=(Undocumented)=-
        void SetDrawable(Ptr<const Animation> Animation)
        {
            mDrawable.SetAnimation(Animation);
            mDrawable.SetState(Drawable::State::Repeat);    // @TODO: Allow fire-once / toggle / repeat
            mDrawable.SetOrigin(Game::Drawable::Pivot::BottomCenter);

            if (Animation)
            {
                SetSize(Vector2f(Animation->Width, Animation->Height));
            }
            else
            {
                SetSize(Vector2f(0.0f, 0.0f));
            }
        }

        // -=(Undocumented)=-
        Ref<Drawable> GetDrawable()
        {
            return mDrawable;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Drawable mDrawable;
    };
}