
#include <Engine/Kernel.hpp>
#include "Game/World.hpp"
#include "Endpoint/LobbyProtocol.hpp"
#include "Game/Controller.hpp"

static constexpr UInt WINDOW_WIDTH  = 1280;
static constexpr UInt WINDOW_HEIGHT = 1024;

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
        mInputService->RemoveListener(shared_from_this());

        mEndpoint = nullptr;
    }

    // -=(Undocumented)=-
    void OnInitialize(ConstSPtr<Platform::Window> Display)
    {
        mEndpoint = GetSubsystem<Network::Service>()->Connect("127.0.0.1", 7666);
        mEndpoint->SetProtocol(NewPtr<Endpoint::LobbyProtocol>());

        mWorld = NewPtr<Game::World>();
        mWorld->Initialize(GetContext());

        Display->SetVisible(true);

        mPlatformService = GetSubsystem<Platform::Service>();
        mGraphicService = GetSubsystem<Graphic::Service>();

        mWorld->GetController().SetViewport(WINDOW_WIDTH, WINDOW_HEIGHT);

        // Init Input Service
        mInputService = GetSubsystem<Input::Service>();
        mInputService->AddListener(shared_from_this());
    }

    // -=(Undocumented)=-
    void OnTick() override
    {
        Rectf Viewport {
            0,
            0,
            WINDOW_WIDTH,
            WINDOW_HEIGHT
        };
        mGraphicService->Prepare(Graphic::k_Default, Viewport, Graphic::Clear::All, 0x00000000, 1, 0);

        static Real64 LastTick = mPlatformService->GetTime();
        Real64 Tick = mPlatformService->GetTime();

        mWorld->Update(Tick);

        mGraphicService->Commit(Graphic::k_Default, false);

        if (Tick - LastTick >= 0.025f)
        {
            LastTick = Tick;

            if (mInputService->IsKeyHeld(Input::Key::W))
            {
                mWorld->GetController().Move(0.0f, -1);
            }
            if (mInputService->IsKeyHeld(Input::Key::S))
            {
                mWorld->GetController().Move(0.0f, +1);
            }
            if (mInputService->IsKeyHeld(Input::Key::A))
            {
                mWorld->GetController().Move(-1, 0.0f);
            }
            if (mInputService->IsKeyHeld(Input::Key::D))
            {
                mWorld->GetController().Move(+1, 0.0f);
            }
        }

    }

    // -=(Undocumented)=-
    Bool OnMouseMove(UInt X, UInt Y)
    {
        Rectf Viewport {
            0,
            0,
            WINDOW_WIDTH,
            WINDOW_HEIGHT
        };

        //Vector2f Pos = mActiveCamera.GetWorldCoordinates(Vector2f(X, Y), Viewport);
        //LOG_INFO("World Coord: {} {}", Pos.GetX(), Pos.GetY());

        return true;
    }

    Bool OnMouseUp(Input::Button Button)
    {
        return true;
    }

    Bool OnMouseScroll(SInt X, SInt Y)
    {
        mWorld->GetController().Zoom(Y * 0.25f);
        return true;
    }

    Bool OnKeyDown(Input::Key Key) override
    {
        return true;
    }

private:

    SPtr<Network::Client> mEndpoint;
    SPtr<Game::World> mWorld;
    SPtr<Platform::Service> mPlatformService;
    SPtr<Graphic::Service> mGraphicService;
    SPtr<Input::Service>   mInputService;

};


int main()
{
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