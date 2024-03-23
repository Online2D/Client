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

        // Set the window visible
        Display->SetVisible(true);
    }

    // -=(Undocumented)=-
    void OnTick() override
    {
        static Real64 LastTick = mPlatform->GetTime();
        Real64 Tick = mPlatform->GetTime();

        mWorld->Tick(Tick);

        if (Tick - LastTick >= 0.025f)
        {
            LastTick = Tick;

            constexpr static SInt32   kFast (10);
            constexpr static Vector2i kUp   ( 0, -1);
            constexpr static Vector2i kDown ( 0, +1);
            constexpr static Vector2i KLeft (-1,  0);
            constexpr static Vector2i kRight(+1,  0);

            if (mInput->IsKeyHeld(Input::Key::W)) mWorld->GetDirector().Move(kUp);
            if (mInput->IsKeyHeld(Input::Key::S)) mWorld->GetDirector().Move(kDown);
            if (mInput->IsKeyHeld(Input::Key::A)) mWorld->GetDirector().Move(KLeft);
            if (mInput->IsKeyHeld(Input::Key::D)) mWorld->GetDirector().Move(kRight);

            if (mInput->IsKeyHeld(Input::Key::Up))    mWorld->GetDirector().Move(kUp    * kFast);
            if (mInput->IsKeyHeld(Input::Key::Down))  mWorld->GetDirector().Move(kDown  * kFast);
            if (mInput->IsKeyHeld(Input::Key::Left))  mWorld->GetDirector().Move(KLeft  * kFast);
            if (mInput->IsKeyHeld(Input::Key::Right)) mWorld->GetDirector().Move(kRight * kFast);
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
    static constexpr UInt WINDOW_WIDTH  = 1280;
    static constexpr UInt WINDOW_HEIGHT = 1024;

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
/*
Vector<Game::Animation> mAnimations;

void LoadAnimations(ConstSPtr<Content::Service> Service)
{
    Service->AddLocator("Resources", NewPtr<Content::SystemLocator>("C:\\Users\\Agustin\\Workspace\\Online-MMO-Resources\\"));


    Chunk File = Service->Find(Content::Uri("Resources://Data/Sprites.ind"));

    if (File.HasData())
    {
        Reader InMemoryReader(File.GetSpan<UInt08>());

        InMemoryReader.Skip(4); // Version
        mAnimations.resize(InMemoryReader.ReadUInt32() + 1);

        while (InMemoryReader.GetAvailable() > 0)
        {
            UInt Index = InMemoryReader.ReadUInt32();
            if (Index != 0)
            {
                Ref<Game::Animation> SpriteInfo = mAnimations[Index];

                SInt Frames = InMemoryReader.ReadUInt16();
                if (Frames > 1)
                {
                    while (--Frames >= 0)
                    {
                        UInt ID = InMemoryReader.ReadUInt32();

                        if (mAnimations[ID].Frames.empty())
                        {
                            LOG_WARNING("INVALID?");
                        }
                        else
                        {
                            Ref<Game::Animation> Other = mAnimations[ID];
                            SpriteInfo.File = Other.File;
                            SpriteInfo.Width = Other.Width;
                            SpriteInfo.Height = Other.Height;

                            Ref<Rectf> Frame = SpriteInfo.Frames.emplace_back();
                            Ref<Rectf> OtherD = mAnimations[ID].Frames[0];
                            Frame = OtherD;
                        }

                        mAnimations[ID].Frames.clear();
                    }

                    SpriteInfo.Duration = InMemoryReader.ReadReal32() / 1000.0f;
                }
                else
                {
                    Ref<Rectf> Frame = SpriteInfo.Frames.emplace_back();

                    SpriteInfo.File = InMemoryReader.ReadUInt32();

                    InMemoryReader.Skip(4); // SX SY
                    SpriteInfo.Width = (InMemoryReader.ReadUInt16());
                    SpriteInfo.Height = (InMemoryReader.ReadUInt16());

                    const Real32 SX1 = InMemoryReader.ReadReal32();
                    const Real32 SY1 = InMemoryReader.ReadReal32();
                    const Real32 SX2 = InMemoryReader.ReadReal32();
                    const Real32 SY2 = InMemoryReader.ReadReal32();

                    Frame.Set(SX1, SY1, SX1 + SX2, SY1 + SY2);
                }
            }
        }
    }

    Writer Output;
    Output.WriteInt(mAnimations.size());

    for (int ID = 0; ID < mAnimations.size(); ++ID)
    {
        Ref<Game::Animation> Animation = mAnimations[ID];
        if (Animation.Frames.empty())
        {
            continue;
        }

        Output.WriteInt(ID);
        Output.WriteInt(Animation.File);
        Output.WriteInt(Animation.Width);
        Output.WriteInt(Animation.Height);
        Output.WriteReal32(Animation.Duration);

        Output.WriteInt(Animation.Frames.size());

        for (int i = 0; i < Animation.Frames.size(); ++i)
        {
            Ref<Rectf> Frame = Animation.Frames[i];
            Output.WriteReal32(Frame.GetLeft());
            Output.WriteReal32(Frame.GetTop());
            Output.WriteReal32(Frame.GetRight());
            Output.WriteReal32(Frame.GetBottom());
        }
    }

    Service->Save(Content::Uri("Resources://Data/Animations.bin"), Output.GetData());
}

int main()
{
    static constexpr UInt WINDOW_WIDTH  = 1280;
    static constexpr UInt WINDOW_HEIGHT = 1024;

    Engine::Properties Properties;
    Properties.SetWindowTitle("Online MMO v0.1");
    Properties.SetWindowWidth(WINDOW_WIDTH);
    Properties.SetWindowHeight(WINDOW_HEIGHT);
    Properties.SetWindowMode(false, true);

    Engine::Kernel Kernel;
    Kernel.Initialize(decltype(Kernel)::Mode::Client, Properties);

    LoadAnimations(Kernel.GetSubsystem<Content::Service>());

    return 0;
}*/