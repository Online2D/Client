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
#include "Director.hpp"
#include "Entities.hpp"
#include "Region.hpp"

#include <Content/Service.hpp>
#include <Graphic/Service.hpp>
#include <Graphic/Renderer.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace World
{
    // -=(Undocumented)=-
    class Universe final
    {
    public:

        // -=(Undocumented)=-
        Universe();

        // -=(Undocumented)=-
        void Initialize(Ref<Subsystem::Context> Context);

        // -=(Undocumented)=-
        void Tick(Real64 Time);

        // -=(Undocumented)=-
        void SetActive(Bool Active)
        {
            mActive = Active;
        }

        // -=(Undocumented)=-
        Bool IsActive() const
        {
            return mActive;
        }

        // -=(Undocumented)=-
        Ref<Animator> GetAnimator()
        {
            return mAnimator;
        }

        // -=(Undocumented)=-
        Ref<Director> GetDirector()
        {
            return mDirector;
        }

        // -=(Undocumented)=-
        Ref<Entities> GetEntities()
        {
            return mEntities;
        }

    private:

        // -=(Undocumented)=-
        struct Chunk
        {
            // -=(Undocumented)=-
            Recti        Dimension;

            // -=(Undocumented)=-
            SPtr<Region> Region;
        };

    private:

        // -=(Undocumented)=-
        void OnUpdate(Real64 Delta);

        // -=(Undocumented)=-
        void OnRender(Real64 Delta);

        // -=(Undocumented)=-
        void Compute(Ref<const Recti> Viewport);

        // -=(Undocumented)=-
        void DrawInterface(Real64 Delta);

        // -=(Undocumented)=-
        void DrawTiles(Ref<Region> Region, UInt32 MinX, UInt32 MinY, UInt32 MaxX, UInt32 MaxY);

        // -=(Undocumented)=-
        void DrawEntity(Ref<Entity> Entity);

        // -=(Undocumented)=-
        void DrawSprite(Ref<Drawable> Drawable, Ref<const Vector2f> Position, Real32 Depth, Drawable::Order Order);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Content::Service>  mResources;
        SPtr<Graphic::Renderer> mRenderer;
        SPtr<Graphic::Pipeline> mPipeline;
        SPtr<Graphic::Font>     mFont;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Bool                    mActive;
        Real64                  mTime;
        Animator                mAnimator;
        Director                mDirector;
        Entities                mEntities;
        Vector<Chunk>           mTerrain;
    };
}