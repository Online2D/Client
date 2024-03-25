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

#include "RegionLoader.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Game
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    RegionLoader::RegionLoader(Ref<Animator> Animator, Ref<Entities> Entities)
        : mAnimator { Animator },
          mEntities { Entities }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool RegionLoader::Load(ConstSPtr<class Content::Service> Service, Ref<Chunk> Data, ConstSPtr<Region> Asset)
    {
        Reader Input(Data.GetSpan<UInt08>());

        constexpr UInt32 kMaskBlock = 0b00000001;
        constexpr UInt32 kMaskDecal = 0b00000010;

        const UInt16   ChunkX = Input.ReadInt<UInt16>();
        const UInt16   ChunkY = Input.ReadInt<UInt16>();
        const Vector3f Position(
            ChunkX * Region::kTilesPerRow    * Tile::kDimension,
            ChunkY * Region::kTilesPerColumn * Tile::kDimension,
            0.0f);

        for (UInt32 Y = 0; Y < Region::kTilesPerColumn; ++Y)
        {
            const UInt32 WorldY = Position.GetY() + (Y * Tile::kDimension);

            for (UInt32 X = 0; X < Region::kTilesPerRow; ++X)
            {
                const UInt32 WorldX = Position.GetX() + (X * Tile::kDimension);

                Ref<Tile> Tile = Asset->GetTile(X, Y);

                const UInt08 Flags = Input.ReadUInt8();

                ReadLayer(Input, Tile, Tile::Layer::Floor, WorldX, WorldY);

                if (Flags & kMaskBlock)
                {
                    Tile.SetProperty(Game::Tile::Property::Block, true);
                }

                if (Flags & kMaskDecal)
                {
                    ReadLayer(Input, Tile, Tile::Layer::Decal, WorldX, WorldY);
                }
            }
        }

        for (UInt32 Element = Input.ReadInt<UInt32>(); Element > 0; --Element)
        {
            ConstSPtr<Entity> Entity = mEntities.Load(Input);
            if (Entity)
            {
                Entity->SetPosition(Entity->GetPosition() + Position);
            }
        }
        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void RegionLoader::ReadLayer(Ref<Reader> Input, Ref<Tile> Tile, Tile::Layer Type, Real32 X, Real32 Y)
    {
        const UInt32 ID = Input.ReadInt<UInt32>();
        if (ID)
        {
            Ref<Drawable> Drawable = Tile.GetLayer(Type);
            Drawable.SetAnimation(mAnimator.GetAnimation(ID));
            Drawable.SetPosition(Vector3f(X, Y, CastEnum(Type)));
            Drawable.SetState(Drawable::State::Repeat);
        }
    }
}