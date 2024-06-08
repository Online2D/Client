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

    Application::Application(Ref<Engine::Kernel> Kernel)
        : Host(Kernel)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnStart()
    {
        constexpr static CStr kFolder = "C:\\Program Files\\Argentum\\Data"; // @TESTING

        // Initialize resources
        ConstSPtr<Content::Service> Resources = GetKernel().GetSubsystem<Content::Service>();
        Resources->AddLocator("Resources", NewPtr<Content::SystemLocator>(kFolder));

        // Initialize browser
        ConstSPtr<UI::Service> Browser = GetKernel().GetSubsystem<UI::Service>();
        Browser->Load("Resources://UI/index.html");
        Browser->Register("doExit", [this](CPtr<const UI::Value>) -> UI::Value {
            GetKernel().Exit();
            return UI::Value();
        });

        // Initialize initial activity (Gateway)
        GetKernel().Goto(NewPtr<Gateway>(GetKernel(), * this));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnStop()
    {

    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnTick(Real64 Time)
    {
        const Rectf Viewport(
            Vector2f(0.0f, 0.0f), GetKernel().GetSubsystem<Platform::Service>()->GetWindow()->GetSize());

        ConstSPtr<Graphic::Service> Graphics = GetKernel().GetSubsystem<Graphic::Service>();
        Graphics->Prepare(Graphic::k_Default, Viewport, Graphic::Clear::All, 0x00000000, 1.0f, 0);
        GetKernel().GetSubsystem<UI::Service>()->Present();
        Graphics->Commit(Graphic::k_Default, false);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::Connect(CStr Address, UInt32 Port)
    {
        mSession = GetKernel().GetSubsystem<Network::Service>()->Connect(shared_from_this(), Address, Port);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnConnect(ConstSPtr<Network::Client> Session)
    {
        ConstSPtr<Foundation::Activity> Foreground = GetKernel().GetForeground<Foundation::Activity>();
        if (Foreground)
        {
            Foreground->OnConnect(Session);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnDisconnect(ConstSPtr<Network::Client> Session)
    {
        ConstSPtr<Foundation::Activity> Foreground = GetKernel().GetForeground<Foundation::Activity>();
        if (Foreground)
        {
            Foreground->OnDisconnect(Session);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnRead(ConstSPtr<Network::Client> Session, CPtr<UInt08> Bytes)
    {
        ConstSPtr<Foundation::Activity> Foreground = GetKernel().GetForeground<Foundation::Activity>();
        if (Foreground)
        {
            Reader Archive(Bytes);
            Foreground->OnMessage(Session, Archive);
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
    Engine::Kernel Kernel;
    Kernel.Initialize(decltype(Kernel)::Mode::Client, Properties, NewPtr<Foundation::Application>(Kernel));
    Kernel.Run();

    return 0;
}