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

#include "World.hpp"
#include "RegionLoader.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Game
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    World::World()
        : mEntities { mAnimator }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void World::Initialize(Ref<Subsystem::Context> Context)
    {
        // Initialize our custom loader(s)
        mResources = Context.GetSubsystem<Content::Service>();
        mResources->AddLoader(NewPtr<RegionLoader>(mAnimator, mEntities));

        // Initialize our graphics service
        mGraphics  = Context.GetSubsystem<Graphic::Service>();

        // Initialize animator
        mAnimator.Initialize(Context);

        // Initialize our default resource(s)
        mRenderer  = NewUniquePtr<Graphic::Renderer>(Context);
        mPipeline  = mResources->Load<Graphic::Pipeline>("Resources://Pipeline/Sprite.effect");
        mFont      = mResources->Load<Graphic::Font>("Resources://Font/Primary.arfont");
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void World::Tick(Real64 Time)
    {
        const Real64 Delta = Time - mTime;
        mTime = Time;

        OnUpdate(Delta);
        OnRender(Delta);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void World::OnUpdate(Real64 Delta)
    {
        // Update the camera and recalculate visible chunk(s)
        const Bool Dirty = mDirector.Compute(Delta);
        if (Dirty)
        {
            Ref<const Recti> Boundaries = Recti::Max(mDirector.GetBoundaries(), Recti(0, 0, 0, 0));
            Compute(Boundaries);
        }

        // TODO: Tick entities
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void World::OnRender(Real64 Delta)
    {
        Rectf Viewport(Vector2f(0.0f, 0.0f), mDirector.GetViewport());

        // Apply default technique
        mGraphics->Prepare(Graphic::k_Default, Viewport, Graphic::Clear::All, 0x00000000, 1.0f, 0);
        {
            // Draw Game
            mRenderer->Begin(mDirector.GetMatrix(), Delta);
            {
                // @TODO: Draw Foreground

                // Draw Middle (Entities)
                mEntities.Query(Viewport * Tile::kDimension, [this](Ref<Entity> Entity)
                {
                    DrawEntity(Entity);
                });
                mRenderer->Flush();

                // Draw Background (Floor, Decal, ...)
                for (Ref<const Chunk> Chunk : mChunks)
                {
                    const UInt32 MinLocalX = Chunk.Dimension.GetLeft();
                    const UInt32 MinLocalY = Chunk.Dimension.GetTop();
                    const UInt32 MaxLocalX = Chunk.Dimension.GetRight();
                    const UInt32 MaxLocalY = Chunk.Dimension.GetBottom();
                    DrawTiles(* Chunk.Region, MinLocalX, MinLocalY, MaxLocalX, MaxLocalY);
                }
            }
            mRenderer->End();

            // Draw Interface (UI)
            DrawInterface(Delta);
        }
        mGraphics->Commit(Graphic::k_Default, false);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void World::Compute(Ref<const Recti> Viewport)
    {
        const UInt32 RegionX1 = Viewport.GetLeft() / Region::kTilesPerRow;
        const UInt32 RegionY1 = Viewport.GetTop()  / Region::kTilesPerColumn;
        const UInt32 RegionX2 = std::ceilf(Viewport.GetRight()  / Region::kTilesPerRow);
        const UInt32 RegionY2 = std::ceilf(Viewport.GetBottom() / Region::kTilesPerColumn);

        UInt32 MinLocalX = Viewport.GetLeft() % Region::kTilesPerRow;
        UInt32 MinLocalY = Viewport.GetTop()  % Region::kTilesPerColumn;

        mChunks.clear(); // @TODO: Should we dispose it immediately or let the content manager do it?

        for (UInt32 RegionY = RegionY1; RegionY <= RegionY2; ++RegionY)
        {
            const UInt32 MaxLocalY = Core::Min(
                Region::kTilesPerColumn, Viewport.GetBottom() - (Region::kTilesPerColumn - 1) * RegionY);

            for (UInt32 RegionX = RegionX1; RegionX <= RegionX2; ++RegionX)
            {
                const UInt32 MaxLocalX = Core::Min(
                    Region::kTilesPerRow, Viewport.GetRight() - (Region::kTilesPerRow - 1) * RegionX);

                const SStr Filename = Format("Resources://World/{}-{}.region", RegionX, RegionY);

                if (ConstSPtr<Region> Region = mResources->Load<Game::Region>(Content::Uri(Filename)))
                {
                    mChunks.emplace_back(Recti(MinLocalX, MinLocalY, MaxLocalX, MaxLocalY), Region);
                }

                MinLocalX = 0;
            }

            MinLocalY = 0;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void World::DrawInterface(Real64 Delta)
    {
        Graphic::Camera Camera;
        Camera.SetOrthographic(mDirector.GetViewport().GetX(), mDirector.GetViewport().GetY(), 0.0f, 1.0f);
        Camera.Compute();

        mRenderer->Begin(Camera.GetWorld(), Delta);
        {
            Ref<const Recti> Boundaries = mDirector.GetBoundaries();
            const SStr16 Coordinates  = Format(L"X: {}/{} Y: {}/{} Zoom: {}",
                Boundaries.GetLeft(),
                Boundaries.GetRight(),
                Boundaries.GetTop(),
                Boundaries.GetBottom(), mDirector.GetZoom());
            mRenderer->DrawFont(mFont, Coordinates, Vector2f(0, 0), 0.0f, 32, -1, Graphic::Renderer::Alignment::LeftTop);
        }
        mRenderer->End();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void World::DrawTiles(Ref<Region> Region, UInt32 MinX, UInt32 MinY, UInt32 MaxX, UInt32 MaxY)
    {
        for (UInt TileY = MinY; TileY < MaxY; ++TileY)
        {
            for (UInt TileX = MinX; TileX < MaxX; ++TileX)
            {
                Ref<Tile> Tile = Region.GetTile(TileX, TileY);
                DrawSprite(Tile.GetLayer(Tile::Layer::Floor), Drawable::Order::Background);
                DrawSprite(Tile.GetLayer(Tile::Layer::Decal), Drawable::Order::Background);
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void World::DrawEntity(Ref<Entity> Entity)
    {
        switch (Entity.GetArchetype())
        {
            case Entity::Type::Object: // @TODO: Improve how Entity works and interact
            {
                Ref<Object> Actor = static_cast<Ref<Object>>(Entity);

                /* @TODO: This is garbage */
                const Rectf Boundaries = Actor.GetBoundaries();
                Actor.GetDrawable().SetPosition(
                    Vector3f(Boundaries.GetLeft(), Boundaries.GetTop(), Actor.GetPosition().GetZ()));

                DrawSprite(Actor.GetDrawable(), Actor.GetPosition(), Drawable::Order::Middle);
            }
            break;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void World::DrawSprite(Ref<Drawable> Drawable, Ref<const Vector3f> Center, Drawable::Order Order)
    {
        Ptr<const Animation> Animation = Drawable.GetAnimation();
        if (Animation)
        {
            ConstSPtr<Graphic::Material> Material = mResources->Retrieve<Graphic::Material>(
                Content::Uri(Format("Memory://Material/{}", Animation->File)), true);

            // @TODO: Remove this and have the content manager load material(s)
            if (Material->GetStatus() == Content::Resource::Status::None)
            {
                ConstSPtr<Graphic::Texture> Texture = mResources->Load<Graphic::Texture>(
                    Content::Uri(Format("Resources://Texture/{}.png", Animation->File)));

                const Graphic::Sampler Sampler(
                    Graphic::TextureEdge::Repeat, Graphic::TextureEdge::Repeat, Graphic::TextureFilter::Nearest);

                Material->SetTexture(0, Texture);
                Material->SetSampler(0, Sampler);

                mResources->Register(Material, false);
            }

            Ref<const Vector3f> Position = Drawable.GetPosition();
            Ref<const Rectf>    Source   = Drawable.GetFrame(mTime);
            Rectf               Destination(
                Position.GetX(),
                Position.GetY(),
                Position.GetX() + Animation->Width,
                Position.GetY() + Animation->Height);

            // @TODO: Check if is possible to have unique depth, if not move this to GPU
            // @TODO: Fixed depth for fixed sprites (Background and Foreground)
            const Vector3f Screen = mDirector.GetSpaceCoordinates(Vector3f(Center.GetX(), Center.GetY(), 0.0f));
            const Real32   Depth  = Director::CalculateDepth(
                CastEnum(Order),
                Screen.GetX(),
                Screen.GetY(),
                Position.GetZ());

            mRenderer->DrawTexture(
                Destination,
                Source,
                Depth,
                Drawable.GetRotation(),
                Graphic::Renderer::Order::Opaque, // @TODO: Customizable
                Drawable.GetColor(),
                mPipeline,                        // @TODO: Customizable
                Material);
        }

    }
}