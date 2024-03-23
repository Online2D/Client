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

#include <Network/Client.hpp>
#include <Network/Protocol.hpp>
#include "LobbyPackets.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Endpoint
{
    // -=(Undocumented)=-
    class LobbyProtocol final : public Network::Protocol
    {
    public:

        // -=(Undocumented)=-
        void OnAttach(ConstSPtr<Network::Client> Client) override;

        // -=(Undocumented)=-
        void OnDetach(ConstSPtr<Network::Client> Client) override;

        // -=(Undocumented)=-
        void OnError(ConstSPtr<Network::Client> Client, UInt Error, CStr Description) override;

        // -=(Undocumented)=-
        void OnRead(ConstSPtr<Network::Client> Client,  CPtr<UInt08> Bytes) override;

        // -=(Undocumented)=-
        void OnWrite(ConstSPtr<Network::Client> Client, CPtr<UInt08> Bytes) override;

    private:

        // -=(Undocumented)=-
        void Handle_LobbyAccountError(ConstSPtr<Network::Client> Client, Ref<const LobbyAccountError> Message);
    };
}