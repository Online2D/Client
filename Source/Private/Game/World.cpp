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

#include "World.hpp"
#include <Content/Locator/SystemLocator.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Game
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void World::Initialize(Ref<Subsystem::Context> Context)
    {
        mContentService = Context.GetSubsystem<Content::Service>();
        mContentService->AddLocator("Resources", NewPtr<Content::SystemLocator>("Resources/"));
        mContentService->AddLoader(NewPtr<RegionLoader>(mAnimator));

        mGraphicService = Context.GetSubsystem<Graphic::Service>();
        mRenderer = NewUniquePtr<Graphic::Renderer>(Context);

        mPipeline = mContentService->Load<Graphic::Pipeline>("Resources://Pipeline/Sprite.effect");
        mFont = mContentService->Load<Graphic::Font>("Resources://Font/Primary.arfont");

        mAnimator.Initialize(Context);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void World::Update(Real64 Time)
    {
        OnRender(Time);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void World::OnRender(Real64 Time)
    {
        const Real32 Difference = Time - mTime;
        mTime = Time;

        // Game
        mController.Tick(Difference);

        mRenderer->Begin(mController.GetCamera().GetWorld(), Difference);

        Ref<const Recti> Viewport = mController.GetViewport();
        Draw(
            std::max(Viewport.GetLeft(), 0),
            std::max(Viewport.GetTop(), 0),
            std::max(Viewport.GetRight(), 0),
            std::max(Viewport.GetBottom(), 0));

        mRenderer->End();

        // UI

        Graphic::Camera UICamera;
        UICamera.SetOrthographic(1024, 768, -1, 1);
        UICamera.Compute();

        mRenderer->Begin(UICamera.GetWorld(), Time);
            const SStr16 Coordinates
                = Format(L"X: {}/{} Y: {}/{} Zoom: {}", Viewport.GetLeft(), Viewport.GetRight(), Viewport.GetTop(), Viewport.GetBottom(), mController.GetZoom());
            mRenderer->DrawFont(mFont, Coordinates, Vector2f(0, 0), 0, 32, -1, Graphic::Renderer::Alignment::LeftTop);
        mRenderer->End();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void World::Draw(UInt WorldMinX, UInt WorldMinY, UInt WorldMaxX, UInt WorldMaxY)
    {
        const UInt RegionX1 = WorldMinX / Region::kTilesPerRowColumn;
        const UInt RegionY1 = WorldMinY / Region::kTilesPerRowColumn;
        const UInt RegionX2 = std::ceilf(WorldMaxX / Region::kTilesPerRowColumn);
        const UInt RegionY2 = std::ceilf(WorldMaxY / Region::kTilesPerRowColumn);

        UInt MinLocalX = WorldMinX % Region::kTilesPerRowColumn;
        UInt MinLocalY = WorldMinY % Region::kTilesPerRowColumn;

        for (UInt RegionY = RegionY1; RegionY <= RegionY2; ++RegionY)
        {
            const UInt MaxLocalY = Core::Min(
                Region::kTilesPerRowColumn, WorldMaxY - (Region::kTilesPerRowColumn - 1) * RegionY);

            for (UInt RegionX = RegionX1; RegionX <= RegionX2; ++RegionX)
            {
                const UInt MaxLocalX = Core::Min(
                    Region::kTilesPerRowColumn, WorldMaxX - (Region::kTilesPerRowColumn - 1) * RegionX);

                if (ConstSPtr<Region> Region = GetRegion(RegionX, RegionY))
                {
                    for (UInt TileY = MinLocalY; TileY < MaxLocalY; ++TileY)
                    {
                        for (UInt TileX = MinLocalX; TileX < MaxLocalX; ++TileX)
                        {
                            Ref<Tile> Tile = Region->GetTile(TileX, TileY);
                            Draw(Tile.GetLayer(Tile::Layer::Floor));
                            Draw(Tile.GetLayer(Tile::Layer::Decal));
                        }
                    }
                }

                MinLocalX = 0;
            }

            MinLocalY = 0;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void World::Draw(Ref<Drawable> Drawable)
    {
        Ptr<const Animation> Animation = Drawable.GetAnimation();
        if (Animation)
        {
            ConstSPtr<Graphic::Material> Material = mContentService->Retrieve<Graphic::Material>(
                Content::Uri(Format("Memory://Material/{}", Animation->File)), true);

            if (Material->GetStatus() == Content::Resource::Status::None)
            {
                ConstSPtr<Graphic::Texture> Texture = mContentService->Load<Graphic::Texture>(
                    Content::Uri(Format("Resources://Texture/{}.png", Animation->File)));

                Material->SetTexture(0, Texture);
                Material->SetSampler(0,
                    Graphic::Sampler(Graphic::TextureEdge::Repeat,
                                     Graphic::TextureEdge::Repeat,
                                     Graphic::TextureFilter::Nearest));

                mContentService->Register(Material, false);
            }

            Ref<const Vector3f> Position = Drawable.GetPosition();
            Ref<const Rectf> Source      = Drawable.GetFrame(mTime);
            Rectf            Destination(
                Position.GetX(),
                Position.GetY(),
                Position.GetX() + Animation->Width,
                Position.GetY() + Animation->Height);

            mRenderer->DrawTexture(
                Destination,
                Source,
                Position.GetZ(),
                Drawable.GetRotation(),
                Graphic::Renderer::Order::Opaque, // TODO
                Drawable.GetColor(),
                mPipeline,
                Material);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    SPtr<Region> World::GetRegion(UInt ChunkX, UInt ChunkY)
    {
        SStr Filename = Format("Resources://World/{}-{}.region", ChunkX, ChunkY);
        return mContentService->Load<Region>(Content::Uri(Filename));
    }

}