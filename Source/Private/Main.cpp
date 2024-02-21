
#include <Engine/Kernel.hpp>
#include "Endpoint/LobbyProtocol.hpp"

// -=(Undocumented)=-
class GameClient : public EnableSmartPointer<GameClient>, public Core::Subsystem
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
        mEndpoint = nullptr;
    }

    // -=(Undocumented)=-
    void OnInitialize()
    {
        mEndpoint = GetSubsystem<Network::Service>()->Connect("127.0.0.1", 7666);
        mEndpoint->SetProtocol(NewPtr<Endpoint::LobbyProtocol>());
    }

    // -=(Undocumented)=-
    void OnTick() override
    {
    }

private:

    SPtr<Network::Client> mEndpoint;
};


int main()
{
    Engine::Properties Properties;
    Properties.SetWindowTitle("Online MMO v0.1");
    Properties.SetWindowWidth(1024);
    Properties.SetWindowHeight(768);
    Properties.SetWindowMode(false, true);

    Engine::Kernel Kernel;
    Kernel.Initialize(decltype(Kernel)::Mode::Client, Properties);
    Kernel.AddSubsystem<GameClient>();
    Kernel.GetSubsystem<GameClient>()->OnInitialize();

    while (Kernel.GetDisplay()->Poll())
    {
        Kernel.Tick();
    }

    Kernel.GetSubsystem<GameClient>()->OnDestroy();


    return 0;
}