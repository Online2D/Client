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

        const UInt32   WorldX = Input.ReadInt<UInt16>() * Region::kTilesPerRow    * Tile::kDimension;
        const UInt32   WorldY = Input.ReadInt<UInt16>() * Region::kTilesPerColumn * Tile::kDimension;
        const Vector2f WorldCoordinates(WorldX, WorldY);

        Asset->SetPosition(WorldCoordinates);

        for (UInt32 Y = 0; Y < Region::kTilesPerColumn; ++Y)
        {
            for (UInt32 X = 0; X < Region::kTilesPerRow; ++X)
            {
                Ref<Tile> Tile = Asset->GetTile(X, Y);

                const UInt08 Flags = Input.ReadUInt8();

                ReadLayer(Input, Tile, Tile::Layer::Floor);

                if (Flags & kMaskBlock)
                {
                    Tile.SetProperty(Game::Tile::Property::Block);
                }

                if (Flags & kMaskDecal)
                {
                    ReadLayer(Input, Tile, Tile::Layer::Decal);
                }
            }
        }

        for (UInt32 Element = Input.ReadInt<UInt32>(); Element > 0; --Element)
        {
            ConstSPtr<Entity> Entity = mEntities.Load(Input);
            if (Entity)
            {
                Entity->SetPosition(Entity->GetPosition() + WorldCoordinates);
            }
        }
        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void RegionLoader::ReadLayer(Ref<Reader> Input, Ref<Tile> Tile, Tile::Layer Type)
    {
        const UInt32 ID = Input.ReadInt<UInt32>();
        if (ID)
        {
            Ref<Drawable> Drawable = Tile.GetLayer(Type);
            Drawable.SetAnimation(mAnimator.GetAnimation(ID));
            Drawable.SetState(Drawable::State::Repeat);
        }
    }
}