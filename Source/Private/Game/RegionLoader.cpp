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

#include "RegionLoader.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Game
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    RegionLoader::RegionLoader(Ref<Animator> Animator)
        : mAnimator { Animator }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool RegionLoader::Load(ConstSPtr<class Content::Service> Service, Ref<Chunk> Data, ConstSPtr<Region> Asset)
    {
        Reader Input(Data.GetSpan<UInt08>());

        const UInt ChunkX = Input.ReadUInt16();
        const UInt ChunkY = Input.ReadUInt16();

        for (UInt Y = 0; Y < Region::kTilesPerRowColumn; ++Y)
        {
            const UInt WorldY = (ChunkY * Region::kTilesPerRowColumn + Y);

            for (UInt X = 0; X < Region::kTilesPerRowColumn; ++X)
            {
                const UInt WorldX = (ChunkX * Region::kTilesPerRowColumn + X);

                Ref<Tile> Tile = Asset->GetTile(X, Y);

                const UInt Flags = Input.ReadUInt8();

                ReadLayer(Input, Tile, Tile::Layer::Floor, WorldX, WorldY);

                if (Flags & 0x02)
                {
                    ReadLayer(Input, Tile, Tile::Layer::Decal, WorldX, WorldY);
                }

                if (Flags & 0x04)
                {
                    Input.Skip(4); // TODO: Remove legacy format
                }

                if (Flags & 0x08)
                {
                    Input.Skip(4); // TODO: Remove legacy format
                }
            }
        }
        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void RegionLoader::ReadLayer(Ref<Reader> Input, Ref<Tile> Tile, Tile::Layer Type, Real32 X, Real32 Y)
    {
        const UInt ID = Input.ReadUInt32();
        if (ID)
        {
            Ref<Drawable> Drawable = Tile.GetLayer(Type);
            Drawable.SetAnimation(mAnimator.GetAnimation(ID));
            Drawable.SetPosition(
                Vector3f(X * Tile::kSizeInPixels,
                         Y * Tile::kSizeInPixels,
                         0 /* TODO */));
            Drawable.SetState(Drawable::State::Repeat);
        }
    }
}