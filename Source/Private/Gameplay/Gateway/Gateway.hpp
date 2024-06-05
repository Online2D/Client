// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2024 by Agustin L. Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "GatewayPackets.hpp"
#include <Engine/Kernel.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Gameplay
{
    // -=(Undocumented)=-
    class Gateway final : public EnableSmartPointer<Gateway>, public Engine::Activity, public Network::Protocol
    {
    public:

        // -=(Undocumented)=-
        static constexpr CStr kRemoteAddress = "127.0.0.1";

        // -=(Undocumented)=-
        static constexpr UInt kRemotePort    = 7666;

    public:

        // -=(Undocumented)=-
        enum class State
        {
            Idle,
            Authenticate,
            Create,
            Waiting,
        };

    public:

        // -=(Undocumented)=-
        Gateway(Ref<Subsystem::Context> Context);

        // \see Activity::OnAttach
        void OnAttach() override;

        // \see Activity::OnDetach
        void OnDetach() override;

        // \see Activity::OnResume
        void OnResume() override;

    private:

        // \see Protocol::OnConnect
        void OnConnect(ConstSPtr<Network::Client> Session) override;

        // \see Protocol::OnDisconnect
        void OnDisconnect(ConstSPtr<Network::Client> Session) override;

        // \see Protocol::OnRead
        void OnRead(ConstSPtr<Network::Client> Client, CPtr<UInt08> Bytes) override;

    private:

        // -=(Undocumented)=-
        void OnAccountLogin(CStr Username, CStr Password);

        // -=(Undocumented)=-
        void OnAccountCreate(CStr Username, CStr Password, CStr Email);

        // -=(Undocumented)=-
        void OnMessage(ConstSPtr<Network::Client> Client, Ref<const GatewayAccountError> Message);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Network::Client> mConnection;
        State                 mState;
        SStr                  mUsername;
        SStr                  mPassword;
        SStr                  mEmail;
    };
}