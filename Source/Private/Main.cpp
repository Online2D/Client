
#include <Engine/Kernel.hpp>
#include "Endpoint/LobbyPackets.hpp"

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
        mEndpoint->Flush();
    }

private:

    // -=(Undocumented)=-
    void OnAttach(ConstSPtr<Network::Client> Client) override
    {
        LOG_INFO("GameClient::OnAttach");

        Client->Write(Endpoint::LobbyAccountLogin("Wolftein", "WhyUCare?"));
        Client->Write(Endpoint::LobbyAccountRegister("Wolftein", "WhyUCare?", "woot@gmail.com"));
        //Client->Write(Endpoint::LobbyAccountDelete("Wolftein"));
    }

    // -=(Undocumented)=-
    void OnDetach(ConstSPtr<Network::Client> Client) override
    {
        LOG_INFO("GameClient::OnDetach");
    }

    // -=(Undocumented)=-
    void OnError(ConstSPtr<Network::Client> Client, UInt Error, CStr Description) override
    {
        LOG_INFO("GameClient::OnError {}:{}", Error, Description);
    }

    // -=(Undocumented)=-
    void OnRead(ConstSPtr<Network::Client> Client,  CPtr<UInt08> Bytes) override
    {
        LOG_INFO("GameClient::OnRead");

        switch (Reader Serializer(Bytes); Serializer.ReadInt<UInt>())
        {
        }
    }

    // -=(Undocumented)=-
    void OnWrite(ConstSPtr<Network::Client> Client, CPtr<UInt08> Bytes) override
    {
        LOG_INFO("GameClient::OnWrite");
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