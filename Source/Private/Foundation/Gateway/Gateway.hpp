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
#include <Foundation/Activity.hpp>

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
        static constexpr UInt kRemotePort    = 7667;

    public:

        // -=(Undocumented)=-
        enum class State
        {
            Idle,
            Authenticate,
            Create,
            Wait,
        };

    public:

        // -=(Undocumented)=-
        Gateway(Ref<Application> Application);

        // \see Activity::OnAttach
        void OnAttach() override;

        // \see Activity::OnDetach
        void OnDetach() override;

        // \see Activity::OnPause
        void OnPause() override;

        // \see Activity::OnResume
        void OnResume() override;

        // \see Activity::OnConnect
        void OnConnect(ConstSPtr<Network::Client> Connection) override;

        // \see Activity::OnMessage
        void OnMessage(ConstSPtr<Network::Client> Connection, Ref<Reader> Archive) override;

        // \see Activity::OnDisconnect
        void OnDisconnect(ConstSPtr<Network::Client> Connection) override;

    private:

        // -=(Undocumented)=-
        void OnAccountLogin(CStr Username, CStr Password);

        // -=(Undocumented)=-
        void OnAccountCreate(CStr Username, CStr Password, CStr Email);

        // -=(Undocumented)=-
        void OnAccountError(Ref<const GatewayAccountError> Message);

        // -=(Undocumented)=-
        void OnAccountAuthorized(Ref<const GatewayAccountData> Message);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        State mState;
        SStr  mUsername;
        SStr  mPassword;
        SStr  mEmail;
    };
}