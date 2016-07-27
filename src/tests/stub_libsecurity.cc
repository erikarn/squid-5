/*
 * Copyright (C) 1996-2016 The Squid Software Foundation and contributors
 *
 * Squid software is distributed under GPLv2+ license and includes
 * contributions from numerous individuals and organizations.
 * Please see the COPYING and CONTRIBUTORS files for details.
 */

#include "squid.h"
#include "AccessLogEntry.h"
#include "comm/Connection.h"
#include "HttpRequest.h"

#define STUB_API "security/libsecurity.la"
#include "tests/STUB.h"

#include "security/EncryptorAnswer.h"
Security::EncryptorAnswer::~EncryptorAnswer() {}
std::ostream &Security::operator <<(std::ostream &os, const Security::EncryptorAnswer &) STUB_RETVAL(os)

#include "security/PeerOptions.h"
Security::PeerOptions Security::ProxyOutgoingConfig;
void Security::PeerOptions::parse(char const*) STUB
Security::ContextPtr Security::PeerOptions::createClientContext(bool) STUB_RETVAL(NULL)
void Security::PeerOptions::updateTlsVersionLimits() STUB
Security::ContextPtr Security::PeerOptions::createBlankContext() const STUB
void Security::PeerOptions::updateContextCa(Security::ContextPtr &) STUB
void Security::PeerOptions::updateContextCrl(Security::ContextPtr &) STUB
void Security::PeerOptions::dumpCfg(Packable*, char const*) const STUB
long Security::PeerOptions::parseOptions() STUB_RETVAL(0)
long Security::PeerOptions::parseFlags() STUB_RETVAL(0)
void parse_securePeerOptions(Security::PeerOptions *) STUB

#include "security/ServerOptions.h"
//Security::ServerOptions::ServerOptions(const Security::ServerOptions &) STUB
void Security::ServerOptions::parse(const char *) STUB
void Security::ServerOptions::dumpCfg(Packable *, const char *) const STUB
Security::ContextPtr Security::ServerOptions::createBlankContext() const STUB
bool Security::ServerOptions::createStaticServerContext(AnyP::PortCfg &) STUB_RETVAL(false)
void Security::ServerOptions::updateContextEecdh(Security::ContextPtr &) STUB

#include "security/NegotiationHistory.h"
Security::NegotiationHistory::NegotiationHistory() STUB
void Security::NegotiationHistory::retrieveNegotiatedInfo(Security::SessionPtr) STUB
void Security::NegotiationHistory::retrieveParsedInfo(Security::TlsDetails::Pointer const &) STUB
const char *Security::NegotiationHistory::cipherName() const STUB
const char *Security::NegotiationHistory::printTlsVersion(AnyP::ProtocolVersion const &v) const STUB

#include "security/Handshake.h"
Security::HandshakeParser::HandshakeParser() STUB
bool Security::HandshakeParser::parseHello(const SBuf &) STUB_RETVAL(false)

#include "security/PeerConnector.h"
CBDATA_NAMESPACED_CLASS_INIT(Security, PeerConnector);
namespace Security
{
PeerConnector::PeerConnector(const Comm::ConnectionPointer &, AsyncCall::Pointer &, const AccessLogEntryPointer &, const time_t) :
    AsyncJob("Security::PeerConnector") {STUB}
PeerConnector::~PeerConnector() {STUB}
void PeerConnector::start() STUB
bool PeerConnector::doneAll() const STUB_RETVAL(true)
void PeerConnector::swanSong() STUB
const char *PeerConnector::status() const STUB_RETVAL("")
void PeerConnector::commCloseHandler(const CommCloseCbParams &) STUB
void PeerConnector::connectionClosed(const char *) STUB
bool PeerConnector::prepareSocket() STUB_RETVAL(false)
void PeerConnector::setReadTimeout() STUB
bool PeerConnector::initializeTls(Security::SessionPointer &) STUB_RETVAL(false)
void PeerConnector::negotiateSsl() STUB
bool PeerConnector::sslFinalized() STUB_RETVAL(false)
void PeerConnector::handleNegotiateError(const int) STUB
void PeerConnector::noteWantRead() STUB
void PeerConnector::noteWantWrite() STUB
void PeerConnector::noteSslNegotiationError(const int, const int, const int) STUB
//    virtual Security::ContextPtr getSslContext() = 0;
void PeerConnector::bail(ErrorState *) STUB
void PeerConnector::callBack() STUB
void PeerConnector::recordNegotiationDetails() STUB
}

#include "security/BlindPeerConnector.h"
CBDATA_NAMESPACED_CLASS_INIT(Security, BlindPeerConnector);
namespace Security
{
bool BlindPeerConnector::initializeTls(Security::SessionPointer &) STUB_RETVAL(false)
Security::ContextPtr BlindPeerConnector::getSslContext() STUB_RETVAL(nullptr)
void BlindPeerConnector::noteNegotiationDone(ErrorState *) STUB
}

