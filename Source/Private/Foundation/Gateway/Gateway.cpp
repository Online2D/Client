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

#include "Gateway.hpp"
#include "Foundation/Lobby/Lobby.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Foundation
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Gateway::Gateway(Ref<Application> Application)
        : Activity(Application)
    {
        mState = State::Idle;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnAttach()
    {
        ConstSPtr<UI::Service> Browser = GetApplication().GetSubsystem<UI::Service>();

        Browser->Register("doAccountLogin", [this](CPtr<const UI::Value> Parameters)
        {
            OnAccountLogin(Parameters[0], Parameters[1]);
            return UI::Value();
        });
        Browser->Register("doAccountCreate", [this](CPtr<const UI::Value> Parameters)
        {
            OnAccountCreate(Parameters[0], Parameters[1], Parameters[2]);
            return UI::Value();
        });
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnDetach()
    {
        ConstSPtr<UI::Service> Browser = GetApplication().GetSubsystem<UI::Service>();
        Browser->Unregister("doAccountLogin");
        Browser->Unregister("doAccountCreate");
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnPause()
    {
        mState = State::Idle;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnResume()
    {
        ConstSPtr<UI::Service> Browser = GetApplication().GetSubsystem<UI::Service>();
        Browser->Call("setMainScreen", "templates/connect.html");
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnConnect(ConstSPtr<Network::Client> Connection)
    {
        switch (mState)
        {
        case State::Idle:
            break;
        case State::Authenticate:
            Connection->Write(GatewayAccountLogin(mUsername, mPassword), false);
            break;
        case State::Create:
            Connection->Write(GatewayAccountRegister(mUsername, mPassword, mEmail), false);
            break;
        case State::Wait:
            break;
        }

        mState = State::Wait;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnMessage(ConstSPtr<Network::Client> Connection, Ref<Reader> Archive)
    {
        switch (Archive.ReadInt<UInt>())
        {
        case GatewayAccountError::kID:
            OnAccountError(GatewayAccountError(Archive));
            break;
        case GatewayAccountData::kID:
            OnAccountAuthorized(GatewayAccountData(Archive));
            break;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnDisconnect(ConstSPtr<Network::Client> Connection)
    {
        ConstSPtr<UI::Service> Browser = GetApplication().GetSubsystem<UI::Service>();

        switch (mState)
        {
        case State::Authenticate:
            Browser->Call("showAccountLoginError", "global.error_connection");
            break;
        case State::Create:
            Browser->Call("showAccountCreateError", "global.error_connection");
            break;
        default:
            break;
        }

        mState = State::Idle;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnAccountLogin(CStr Username, CStr Password)
    {
        if (mState != State::Idle)
        {
            return;
        }

        mState    = State::Authenticate;
        mUsername = Username;
        mPassword = Password;

        GetApplication().Connect(kRemoteAddress, kRemotePort);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnAccountCreate(CStr Username, CStr Password, CStr Email)
    {
        if (mState != State::Idle)
        {
            return;
        }

        mState    = State::Create;
        mUsername = Username;
        mPassword = Password;
        mEmail    = Email;

        GetApplication().Connect(kRemoteAddress, kRemotePort);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnAccountError(Ref<const GatewayAccountError> Message)
    {
        ConstSPtr<UI::Service> Browser = GetApplication().GetSubsystem<UI::Service>();

        switch (Message.Type)
        {
        case GatewayAccountError::ID::Unknown:
            Browser->Call("showAccountLoginError", "connect.error_account_unknown");
            break;
        case GatewayAccountError::ID::Mismatch:
            Browser->Call("showAccountLoginError", "connect.error_account_mismatch");
            break;
        case GatewayAccountError::ID::Exist:
            Browser->Call("showAccountCreateError", "create_account.error_account_exist");
            break;
        case GatewayAccountError::ID::Online:
            Browser->Call("showAccountLoginError", "connect.error_account_online");
            break;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnAccountAuthorized(Ref<const GatewayAccountData> Message)
    {
        // Switch to the next activity (Lobby)
        GetApplication().Goto(NewPtr<Lobby>(GetApplication()));

        // Display the character(s) information
        ConstSPtr<UI::Service> Browser = GetApplication().GetSubsystem<UI::Service>();

        for (Ref<const GatewayAccountData::Entity> Entity : Message.Entities)
        {
            Browser->Call("setCharacterInLobby",
                Entity.ID,
                Entity.Name,
                Entity.Level,
                Entity.Class,
                Entity.Location);
        }
    }
}