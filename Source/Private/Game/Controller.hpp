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

#include "Graphic/Camera.hpp"
#include "Tile.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Game
{
    // -=(Undocumented)=-
    class Controller final
    {
    public:

        // -=(Undocumented)=-
        static constexpr Real32 kMinimumZoom = 0.25f;

        // -=(Undocumented)=-
        static constexpr Real32 kMaximumZoom = 32.0f;

    public:

        // -=(Undocumented)=-
        Controller();

        // -=(Undocumented)=-
        void Tick(Real32 Delta);

        // -=(Undocumented)=-
        void SetViewport(UInt32 Width, UInt32 Height)
        {
            mSize.Set(Width, Height);

            const Real32 HalfWidth  = (mSize.GetX() * 0.5f) * mZoom;
            const Real32 HalfHeight = (mSize.GetY() * 0.5f) * mZoom;
            mCamera.SetOrthographic(-HalfWidth, HalfWidth, HalfHeight, -HalfHeight, -1, 1);
        }

        // -=(Undocumented)=-
        Ref<const Recti> GetViewport() const
        {
            return mViewport;
        }

        // -=(Undocumented)=-
        void SetPosition(Vector2i Position)
        {
            mCamera.SetPosition(Vector2f(Position.GetX(), Position.GetY()) * Tile::kSizeInPixels);
        }

        // -=(Undocumented)=-
        Vector2i GetPosition() const
        {
            Ref<const Vector3f> Position = mCamera.GetPosition();
            return Vector2i(Position.GetX() / Tile::kSizeInPixels, Position.GetY() / Tile::kSizeInPixels);
        }

        // -=(Undocumented)=-
        void SetZoom(Real32 Zoom)
        {
            mZoom = Within(Zoom, kMinimumZoom, kMaximumZoom);
            SetViewport(mSize.GetX(), mSize.GetY());
        }

        // -=(Undocumented)=-
        Real32 GetZoom() const
        {
            return mZoom;
        }

        // -=(Undocumented)=-
        void Move(SInt32 X, SInt32 Y) // TODO: Smooth Movement
        {
            mCamera.Translate(Vector2f(X, Y) * Tile::kSizeInPixels);
        }

        // -=(Undocumented)=-
        void Zoom(Real32 Magnitude) // TODO: Smooth Zoom
        {
            SetZoom(mZoom + Magnitude);
        }

        // -=(Undocumented)=-
        Ref<const Graphic::Camera> GetCamera() const
        {
            return mCamera;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Graphic::Camera mCamera;
        Vector2i        mSize;
        Recti           mViewport;
        Real32          mZoom;
    };
}