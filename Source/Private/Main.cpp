
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

        Graphic::Camera Camera;

        mWorld->GetController().SetViewport(WINDOW_WIDTH, WINDOW_HEIGHT);
        //mWorld->GetController().SetPosition(Vector2i(0, 0));

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
        mWorld->GetController().Zoom(Y * 0.2f);
        return true;
    }

    Bool OnKeyDown(Input::Key Key) override
    {
        switch (Key)
        {
        case Input::Key::W:
            mWorld->GetController().Move(0.0f, -1);
            break;
        case Input::Key::S:
            mWorld->GetController().Move(0.0f, +1);
            break;
        case Input::Key::A:
            mWorld->GetController().Move(-1, 0.0f);
            break;
        case Input::Key::D:
            mWorld->GetController().Move(+1, 0.0f);
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