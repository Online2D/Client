
#include <Engine/Kernel.hpp>

// -=(Undocumented)=-
class GameClient : public EnableSmartPointer<GameClient>, public Core::Subsystem, public Network::Protocol
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
        mEndpoint = GetSubsystem<Network::Service>()->Connect("127.0.0.1", "7666");
        mEndpoint->Attach(shared_from_this());
    }

    // -=(Undocumented)=-
    void OnTick() override
    {

    }

private:

    // -=(Undocumented)=-
    void OnAttach(ConstSPtr<Network::Session> Session) override
    {
        LOG_INFO("We connected...");
    }

    // -=(Undocumented)=-
    void OnDetach(ConstSPtr<Network::Session> Session) override
    {
        LOG_INFO("We disconnected...");
    }

    // -=(Undocumented)=-
    void OnError(ConstSPtr<Network::Session> Session, UInt Error, CStr Description) override
    {
        LOG_INFO("Error from the underlying network service '{}'", Description);
    }

    // -=(Undocumented)=-
    void OnRead(ConstSPtr<Network::Session> Session,  CPtr<UInt08> Bytes) override
    {
        LOG_INFO("We recv some data...");
    }

    // -=(Undocumented)=-
    void OnWrite(ConstSPtr<Network::Session> Session, CPtr<UInt08> Bytes) override
    {
        LOG_INFO("We wrote some data...");
    }

private:

    SPtr<Network::Client> mEndpoint;
};


int main()
{
    Engine::Properties Properties;
    Properties.SetWindowTitle("Argentum Online v1.0");
    Properties.SetWindowWidth(1024);
    Properties.SetWindowHeight(768);
    Properties.SetWindowMode(false, true);

    Engine::Kernel Kernel;
    Kernel.Initialize(Properties);
    Kernel.AddSubsystem<GameClient>();
    Kernel.GetSubsystem<GameClient>()->OnInitialize();

    while (Kernel.GetDisplay()->Poll())
    {
        Kernel.Tick();
    }

    Kernel.GetSubsystem<GameClient>()->OnDestroy();


    return 0;
}