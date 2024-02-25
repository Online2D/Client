
#include <Engine/Kernel.hpp>
#include "Game/World.hpp"
#include "Endpoint/LobbyProtocol.hpp"
#include <windows.h>

static constexpr UInt WINDOW_WIDTH  = 1024;
static constexpr UInt WINDOW_HEIGHT = 768;

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

        Graphic::Camera Camera;

        Camera.SetOrthographic(WINDOW_WIDTH, WINDOW_HEIGHT, 1000, -1000);
        Camera.SetPosition(0, 0);
        Camera.Compute();

        mWorld->SetCamera(Camera);

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

        mWorld->Update(mPlatformService->GetTime());

        mGraphicService->Commit(Graphic::k_Default, false);

        auto Input = GetSubsystem<Input::Service>();

        constexpr static Real32 SPEED = 5.0f;

        if (Input->IsKeyHeld(Input::Key::W))
        {
            mWorld->GetCamera().Translate(0.0f, -SPEED);
            mWorld->GetCamera().Compute();
        }
        if (Input->IsKeyHeld(Input::Key::S))
        {
            mWorld->GetCamera().Translate(0.0f, +SPEED);
            mWorld->GetCamera().Compute();
        }
        if (Input->IsKeyHeld(Input::Key::A))
        {
            mWorld->GetCamera().Translate(-SPEED, 0.0f);
            mWorld->GetCamera().Compute();
        }
        if (Input->IsKeyHeld(Input::Key::D))
        {
            mWorld->GetCamera().Translate(+SPEED, 0.0f);
            mWorld->GetCamera().Compute();
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

    Bool OnMouseScroll(SInt X, SInt Y)
    {
        Zoom += Y * 0.01;
        mWorld->GetCamera().SetScale(Zoom, Zoom);
        mWorld->GetCamera().Compute();
        return true;
    }


    Bool OnKeyDown(Input::Key Key) override
    {
        switch (Key)
        {
        case Input::Key::F1:
            mWorld->GetCamera().Rotate(45 , Vector3f(0, 0, 1));
            mWorld->GetCamera().Compute();
            break;
        default:
            return false;
        }
        return true;
    }

private:

    SPtr<Network::Client> mEndpoint;
    SPtr<Game::World> mWorld;
    SPtr<Platform::Service> mPlatformService;
    SPtr<Graphic::Service> mGraphicService;
    SPtr<Input::Service>   mInputService;
    Real32 Zoom = 1.0f;

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