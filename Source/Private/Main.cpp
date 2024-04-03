// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2024 by Agustin L. Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Game/World.hpp"

#include <Engine/Kernel.hpp>
#include <Content/Locator/SystemLocator.hpp>
#include <windows.h>

// -=(Undocumented)=-
class GameClient : public EnableSmartPointer<GameClient>, public Core::Subsystem, public Input::Listener
{
public:

    // -=(Undocumented)=-
    GameClient(Ref<Context> Context)
        : Subsystem { Context }
    {
    }

    // -=(Undocumented)=-
    void OnDestroy()
    {
        mInput->RemoveListener(shared_from_this());
    }

    void AddCustom()
    {
        /*
        Game::Animation Body_1 {
            .ID       = 70004,
            .File     = 70001,
            .Width    = 18,
            .Height   = 38,
            .Frames   = {
                Rectf {   7.0f / 256.0f, 19.0f / 256.0f,  25.0f / 256.0f, 57.0f / 256.0f },
                Rectf {  39.0f / 256.0f, 19.0f / 256.0f,  58.0f / 256.0f, 57.0f / 256.0f },
                Rectf {  71.0f / 256.0f, 19.0f / 256.0f,  90.0f / 256.0f, 57.0f / 256.0f },
                Rectf { 103.0f / 256.0f, 19.0f / 256.0f, 122.0f / 256.0f, 57.0f / 256.0f },
                Rectf { 135.0f / 256.0f, 19.0f / 256.0f, 153.0f / 256.0f, 57.0f / 256.0f },
                Rectf { 166.0f / 256.0f, 19.0f / 256.0f, 185.0f / 256.0f, 57.0f / 256.0f },
                Rectf { 198.0f / 256.0f, 19.0f / 256.0f, 217.0f / 256.0f, 57.0f / 256.0f },
                Rectf { 230.0f / 256.0f, 19.0f / 256.0f, 249.0f / 256.0f, 57.0f / 256.0f },
            },
            .Duration = 1.0f
        };
        mWorld->GetAnimator().AddAnimation(Body_1);

        Game::Animation Head_1 {
            .ID       = 70000,
            .File     = 70000,
            .Width    = 11,
            .Height   = 16,
            .Frames   = {
                Rectf { 3.0f / 68.0f, 0.0f / 16.0f, 14.0f / 68.0f, 16.0f / 16.0f }
            },
            .Duration = 0.0f
        };
        Game::Animation Head_2 {
            .ID       = 70001,
            .File     = 70000,
            .Width    = 11,
            .Height   = 16,
            .Frames   = {
                Rectf { 19.0f / 68.0f, 0.0f / 16.0f, (31.0f + 1) / 68.0f, (15.0f + 1) / 16.0f }
            },
            .Duration = 0.0f
        };
        Game::Animation Head_3 {
            .ID       = 70002,
            .File     = 70000,
            .Width    = 11,
            .Height   = 16,
            .Frames   = {
                Rectf { 36.0f / 68.0f, 0.0f / 16.0f, (48.0f + 1) / 68.0f, (15.0f + 1) / 16.0f }
            },
            .Duration = 0.0f
        };
        Game::Animation Head_4 {
            .ID       = 70003,
            .File     = 70000,
            .Width    = 11,
            .Height   = 16,
            .Frames   = {
                Rectf { 54.0f / 68.0f, 0.0f / 16.0f, (64.0f + 1) / 68.0f, (14.0f + 1) / 16.0f }
            },
            .Duration = 0.0f
        };
        mWorld->GetAnimator().AddAnimation(Head_4);
        mWorld->GetAnimator().AddAnimation(Head_3);
        mWorld->GetAnimator().AddAnimation(Head_2);
        mWorld->GetAnimator().AddAnimation(Head_1);*/
    }

    // -=(Undocumented)=-
    void OnInitialize(ConstSPtr<Platform::Window> Display)
    {
        // Init platform service
        mPlatform = GetSubsystem<Platform::Service>();

        // Initialize our custom locator
        ConstSPtr<Content::Service> Resources = GetSubsystem<Content::Service>();
        Resources->AddLocator("Resources", NewPtr<Content::SystemLocator>("C:\\Users\\Agustin\\Workspace\\Online-MMO-Resources\\"));

        // Init input Service
        mInput = GetSubsystem<Input::Service>();
        mInput->AddListener(shared_from_this());

        // Init world
        mWorld = NewPtr<Game::World>();
        mWorld->Initialize(GetContext());
        mWorld->GetDirector().SetViewport(Display->GetSize());
        mWorld->GetDirector().SetPosition(Vector2i(290, 990));

        // Set the window visible
        Display->SetVisible(true);

        AddCustom();
    }

    // -=(Undocumented)=-
    void OnTick() override
    {
        mWorld->Tick(mPlatform->GetTime());

        constexpr static SInt32   kFast (10);
        constexpr static Vector2i kUp   ( 0, -1);
        constexpr static Vector2i kDown ( 0, +1);
        constexpr static Vector2i kLeft (-1,  0);
        constexpr static Vector2i kRight(+1,  0);

        Vector2i Movement;

             if (mInput->IsKeyHeld(Input::Key::W)) Movement = kUp;
        else if (mInput->IsKeyHeld(Input::Key::S)) Movement = kDown;
             if (mInput->IsKeyHeld(Input::Key::A)) Movement += kLeft;
        else if (mInput->IsKeyHeld(Input::Key::D)) Movement += kRight;

             if (mInput->IsKeyHeld(Input::Key::Up))     Movement = kUp   * kFast;
        else if (mInput->IsKeyHeld(Input::Key::Down))  Movement = kDown * kFast;
             if (mInput->IsKeyHeld(Input::Key::Left))  Movement += kLeft  * kFast;
        else if (mInput->IsKeyHeld(Input::Key::Right)) Movement += kRight * kFast;

        if (! Movement.IsZero())
        {
            mWorld->GetDirector().Move(Movement);
        }
    }

private:

    // -=(Undocumented)=-
    Bool OnMouseScroll(SInt X, SInt Y) override
    {
        mWorld->GetDirector().Zoom(Y * 0.25f);
        return true;
    }

private:

    SPtr<Platform::Service> mPlatform;
    SPtr<Input::Service>    mInput;
    SPtr<Game::World>       mWorld;
};


int main()
{
#ifdef    EA_PLATFORM_WINDOWS
    ::CoInitialize(nullptr);
#endif // EA_PLATFORM_WINDOWS

    static constexpr UInt WINDOW_WIDTH  = 1024;
    static constexpr UInt WINDOW_HEIGHT = 768;

    Engine::Properties Properties;
    Properties.SetWindowTitle("Online MMO v0.1");
    Properties.SetWindowWidth(WINDOW_WIDTH);
    Properties.SetWindowHeight(WINDOW_HEIGHT);
    Properties.SetWindowMode(false, true);

    Engine::Kernel Kernel;
    Kernel.Initialize(decltype(Kernel)::Mode::Client, Properties);

    Kernel.AddSubsystem<GameClient>();
    Kernel.GetSubsystem<GameClient>()->OnInitialize(Kernel.GetDisplay());

    while (Kernel.GetDisplay()->Poll())
    {
        Kernel.Tick();
    }

    Kernel.GetSubsystem<GameClient>()->OnDestroy();


    return 0;
}