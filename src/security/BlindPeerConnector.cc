/*
 * Copyright (C) 1996-2016 The Squid Software Foundation and contributors
 *
 * Squid software is distributed under GPLv2+ license and includes
 * contributions from numerous individuals and organizations.
 * Please see the COPYING and CONTRIBUTORS files for details.
 */

#include "squid.h"
#include "CachePeer.h"
#include "comm/Connection.h"
#include "errorpage.h"
#include "fde.h"
#include "HttpRequest.h"
#include "neighbors.h"
#include "security/BlindPeerConnector.h"
#include "security/NegotiationHistory.h"
#include "SquidConfig.h"

CBDATA_NAMESPACED_CLASS_INIT(Security, BlindPeerConnector);

Security::ContextPtr
Security::BlindPeerConnector::getSslContext()
{
    if (const CachePeer *peer = serverConnection()->getPeer()) {
        assert(peer->secure.encryptTransport);
        Security::ContextPtr sslContext(peer->sslContext);
        return sslContext;
    }
    return ::Config.ssl_client.sslContext;
}

bool
Security::BlindPeerConnector::initialize(Security::SessionPointer &serverSession)
{
    if (!Security::PeerConnector::initialize(serverSession))
        return false;

    if (const CachePeer *peer = serverConnection()->getPeer()) {
        assert(peer);

        // NP: domain may be a raw-IP but it is now always set
        assert(!peer->secure.sslDomain.isEmpty());

#if USE_OPENSSL
        // const loss is okay here, ssl_ex_index_server is only read and not assigned a destructor
        SBuf *host = new SBuf(peer->secure.sslDomain);
        SSL_set_ex_data(serverSession.get(), ssl_ex_index_server, host);

        if (peer->sslSession)
            SSL_set_session(serverSession.get(), peer->sslSession);
    } else {
        SBuf *hostName = new SBuf(request->url.host());
        SSL_set_ex_data(serverSession.get(), ssl_ex_index_server, (void*)hostName);
#endif
    }
    return true;
}

void
Security::BlindPeerConnector::noteNegotiationDone(ErrorState *error)
{
    if (error) {
        // XXX: forward.cc calls peerConnectSucceeded() after an OK TCP connect but
        // we call peerConnectFailed() if SSL failed afterwards. Is that OK?
        // It is not clear whether we should call peerConnectSucceeded/Failed()
        // based on TCP results, SSL results, or both. And the code is probably not
        // consistent in this aspect across tunnelling and forwarding modules.
        if (CachePeer *p = serverConnection()->getPeer())
            peerConnectFailed(p);
        return;
    }

#if USE_OPENSSL
    const int fd = serverConnection()->fd;
    Security::SessionPtr ssl = fd_table[fd].ssl.get();
    if (serverConnection()->getPeer() && !SSL_session_reused(ssl)) {
        if (serverConnection()->getPeer()->sslSession)
            SSL_SESSION_free(serverConnection()->getPeer()->sslSession);

        serverConnection()->getPeer()->sslSession = SSL_get1_session(ssl);
    }
#endif
}

