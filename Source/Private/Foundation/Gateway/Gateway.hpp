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
#include "Foundation/Activity.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Foundation
{
    // -=(Undocumented)=-
    class Gateway final : public Activity
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
        Gateway(Ref<Subsystem::Context> Context, Ref<Application> Application);

        // \see Activity::OnAttach
        void OnAttach() override;

        // \see Activity::OnDetach
        void OnDetach() override;

        // \see Activity::OnResume
        void OnResume() override;

        // \see Activity::OnConnect
        void OnConnect(SPtr<Network::Client> Session) override;

        // \see Activity::OnMessage
        void OnMessage(SPtr<Network::Client> Session, Ref<Reader> Archive) override;

        // \see Activity::OnDisconnect
        void OnDisconnect(SPtr<Network::Client> Session) override;

    private:

        // -=(Undocumented)=-
        void OnAccountLogin(CStr Username, CStr Password);

        // -=(Undocumented)=-
        void OnAccountCreate(CStr Username, CStr Password, CStr Email);

        // -=(Undocumented)=-
        void OnAccountError(ConstSPtr<Network::Client> Client, Ref<const GatewayAccountError> Message);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        State mState;
        SStr  mUsername;
        SStr  mPassword;
        SStr  mEmail;
    };
}