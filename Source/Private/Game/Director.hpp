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

#include "Tile.hpp"
#include <Graphic/Camera.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Game
{
    // -=(Undocumented)=-
    class Director final
    {
    public:

        // -=(Undocumented)=-
        static constexpr Real32 kDelay       = 0.25f;

        // -=(Undocumented)=-
        static constexpr Real32 kMinimumZoom = 0.1f;

        // -=(Undocumented)=-
        static constexpr Real32 kMaximumZoom = 16.0f;

    public:

        // -=(Undocumented)=-
        Director();

        // -=(Undocumented)=-
        Bool Compute(Real64 Delta);

        // -=(Undocumented)=-
        void SetViewport(Vector2i Size)
        {
            mSize.Set(Size.GetX(), Size.GetY());

            const Real32 HalfWidth  = (mSize.GetX() * 0.5f) * mZoom;
            const Real32 HalfHeight = (mSize.GetY() * 0.5f) * mZoom;
            mCamera.SetOrthographic(-HalfWidth, HalfWidth, HalfHeight, -HalfHeight, 0.0f, 1.0f);
        }

        // -=(Undocumented)=-
        Ref<const Vector2i> GetViewport() const
        {
            return mSize;
        }

        // -=(Undocumented)=-
        void SetPosition(Vector2i Position)
        {
            mTranslation = Interpolator<Vector3f>();

            const SInt32 XInPixels  = Position.GetX() * Tile::kDimension;
            const SInt32 YInPixels  = Position.GetY() * Tile::kDimension;
            mCamera.SetPosition(XInPixels, YInPixels);
        }

        // -=(Undocumented)=-
        Vector2i GetPosition() const
        {
            Ref<const Vector3f> Position = mCamera.GetPosition();
            return Vector2i(Position.GetX() / Tile::kDimension, Position.GetY() / Tile::kDimension);
        }

        // -=(Undocumented)=-
        void SetZoom(Real32 Magnitude)
        {
            mMagnitude = Interpolator<Real32>();

            mZoom = Within(Magnitude, kMinimumZoom, kMaximumZoom);
            SetViewport(mSize);
        }

        // -=(Undocumented)=-
        Real32 GetZoom() const
        {
            return mZoom;
        }

        // -=(Undocumented)=-
        void Move(Vector2i Translation)
        {
            if (mTranslation.HasFinish())
            {
                Ref<const Vector3f> Position = mCamera.GetPosition();

                mTranslation = Interpolator<Vector3f>(
                    Position,
                    Position + Vector3f(Translation.GetX(), Translation.GetY(), 0.0f) * Tile::kDimension,
                    kDelay);
            }
        }

        // -=(Undocumented)=-
        void Zoom(Real32 Magnitude)
        {
            if (mMagnitude.HasFinish())
            {
                mMagnitude = Interpolator<Real32>(
                    mZoom, Within(mZoom + Magnitude, kMinimumZoom, kMaximumZoom), kDelay);
            }
        }

        // -=(Undocumented)=-
        Ref<const Recti> GetBoundaries() const
        {
            return mBoundaries;
        }

        // -=(Undocumented)=-
        Vector3f GetWorldCoordinates() const
        {
            return mCamera.GetPosition();
        }

        // -=(Undocumented)=-
        Vector3f GetWorldCoordinates(Ref<const Vector3f> Position)
        {
            return mCamera.GetWorldCoordinates(Position, Rectf(Vector2f(0.0f, 0.0f), mSize));
        }

        // -=(Undocumented)=-
        Vector3f GetSpaceCoordinates(Ref<const Vector3f> Position)
        {
            return mCamera.GetScreenCoordinates(Position, Rectf(Vector2f(0.0f, 0.0f), mSize));
        }

        // -=(Undocumented)=-
        Ref<const Matrix4f> GetMatrix() const
        {
            return mCamera.GetWorld();
        }

    public:

        // -=(Undocumented)=-
        static constexpr Real32 CalculateDepth(UInt64 Value)
        {
            constexpr Real64 kFarPlane = static_cast<Real64>((1ull << 24) - 1);
            return 1.0 - (Value / kFarPlane);
        }

        // -=(Undocumented)=-
        static constexpr Real32 CalculateDepth(UInt32 Layer, UInt32 X, UInt32 Y, UInt32 Z)
        {
            constexpr UInt64 LayerBits     = 2;
            constexpr UInt64 LayerMask     = GetMask<UInt32>(LayerBits);
            constexpr UInt64 YPositionBits = 10;
            constexpr UInt64 YPositionMask = GetMask<UInt32>(YPositionBits);
            constexpr UInt64 XPositionBits = 10;
            constexpr UInt64 XPositionMask = GetMask<UInt32>(XPositionBits);
            constexpr UInt64 ZPositionBits = 2;
            constexpr UInt64 ZPositionMask = GetMask<UInt32>(ZPositionBits);

            const UInt64 Value =
                  (Layer & LayerMask) << (YPositionBits + XPositionBits + ZPositionBits)
                | (Y & YPositionMask) << (XPositionBits + ZPositionBits)
                | (X & XPositionMask) << (ZPositionBits)
                | (Z & ZPositionMask);
            return CalculateDepth(Value);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Graphic::Camera        mCamera;
        Vector2i               mSize;
        Recti                  mBoundaries;
        Real32                 mZoom;
        Interpolator<Vector3f> mTranslation;
        Interpolator<Real32>   mMagnitude;
    };
}