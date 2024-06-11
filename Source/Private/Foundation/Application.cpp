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

#include "Application.hpp"
#include "Gateway/Gateway.hpp"
#include <Content/Locator/SystemLocator.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Foundation
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::Connect(CStr Address, UInt32 Port)
    {
        mSession = GetSubsystem<Network::Service>()->Connect(shared_from_this(), Address, Port);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::Disconnect()
    {
        if (mSession)
        {
            mSession->Close(false);
            mSession = nullptr;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::Goto(SPtr<Activity> Foreground)
    {
        ConstSPtr<Activity> Current = mActivities.back();
        if (Current)
        {
            Current->OnPause();
        }

        mActivities.emplace_back(Foreground);
        Foreground->OnAttach();
        Foreground->OnResume();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::Back()
    {
        ConstSPtr<Activity> Current = mActivities.back();
        if (Current)
        {
            Current->OnPause();
            Current->OnDetach();
            mActivities.pop_back();
        }

        ConstSPtr<Activity> Newest = mActivities.back();
        if (Newest)
        {
            Newest->OnResume();
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Application::OnInitialize()
    {
        constexpr static CStr kFolder = "C:\\Program Files\\Argentum\\Data"; // @TESTING

        // Initialize resources
        ConstSPtr<Content::Service> Resources = GetSubsystem<Content::Service>();
        Resources->AddLocator("Resources", NewPtr<Content::SystemLocator>(kFolder));

        // Initialize browser
        ConstSPtr<UI::Service> Browser = GetSubsystem<UI::Service>();
        Browser->Load("Resources://UI/index.html");
        Browser->Register("doExit", [this](CPtr<const UI::Value>) -> UI::Value {
            Exit();
            return UI::Value();
        });

        // Initialize initial activity (Gateway)
        Goto(NewPtr<Gateway>(* this));
        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnDestroy()
    {

    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnTick(Real64 Time)
    {
        const Rectf Viewport(Vector2f(0.0f, 0.0f), GetSubsystem<Platform::Service>()->GetWindow()->GetSize());

        ConstSPtr<Graphic::Service> Graphics = GetSubsystem<Graphic::Service>();
        Graphics->Prepare(Graphic::k_Default, Viewport, Graphic::Clear::All, 0x00000000, 1.0f, 0);
        {
            // Tick (Activity)
            ConstSPtr<Activity> Foreground = mActivities.back();
            if (Foreground)
            {
                Foreground->OnTick(Time);
            }

            // Tick (UI)
            GetSubsystem<UI::Service>()->Present();
        }
        Graphics->Commit(Graphic::k_Default, false);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnConnect(ConstSPtr<Network::Client> Session)
    {
        ConstSPtr<Activity> Foreground = mActivities.back();
        if (Foreground)
        {
            Foreground->OnConnect(Session);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnDisconnect(ConstSPtr<Network::Client> Session)
    {
        ConstSPtr<Activity> Foreground = mActivities.back();
        if (Foreground)
        {
            Foreground->OnDisconnect(Session);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnRead(ConstSPtr<Network::Client> Session, CPtr<UInt08> Bytes)
    {
        ConstSPtr<Activity> Foreground = mActivities.back();
        if (Foreground)
        {
            Reader Archive(Bytes);
            do
            {
                Foreground->OnMessage(Session, Archive);
            }
            while (Archive.GetAvailable() > 0);
        }
    }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   MAIN   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifdef    EA_PLATFORM_WINDOWS
    #include <Windows.h>
#endif // EA_PLATFORM_WINDOWS

int main([[maybe_unused]] int Argc, [[maybe_unused]] Ptr<Char> Argv[])
{
#ifdef    EA_PLATFORM_WINDOWS
    ::CoInitialize(nullptr);
#endif // EA_PLATFORM_WINDOWS

    Engine::Properties Properties;
    Properties.SetWindowTitle("Argentum v0.1");
    Properties.SetWindowWidth(1024);
    Properties.SetWindowHeight(768);
    Properties.SetWindowMode(false, false);

    // Initialize 'Aurora Engine' and enter main loop
    SPtr<Foundation::Application> Application = NewPtr<Foundation::Application>();
    Application->Initialize(System<Subsystem>::Mode::Client, Properties);
    Application->Run();

    return 0;
}