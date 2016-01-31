/*
 * Copyright (c) 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Patent rights are not granted under this agreement. Patent rights are
 *       available under FRAND terms.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX or PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @file metis_UdpTunnel.h
 * @brief Establish a tunnel to a remote system
 *
 * Creates a "udp tunnel" to a remote system.  There must already be a local UDP listener for the
 * local side of the connection.  Because UDP is connectionless and we do not have a link protocol (case 210),
 * the udp tunnel will go in the connection table immediately in the "up" state.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#ifndef Metis_metis_UdpTunnel_h
#define Metis_metis_UdpTunnel_h

#include <ccnx/api/control/cpi_Address.h>
#include <ccnx/forwarder/metis/core/metis_Forwarder.h>
#include <ccnx/forwarder/metis/io/metis_Listener.h>
#include <ccnx/forwarder/metis/io/metis_IoOperations.h>

/**
 * Establishes a connection to a remote system over UDP
 *
 * The remoteAddress must be of the same type (i.e. v4 or v6) as the localListener.
 *
 * The connection will go in the table immediately, and will be in the "up" state.
 *
 * Caveat: need a link setup protocol (see case 210)
 *
 * @param [in] metis An allocated MetisForwarder
 * @param [in] localListener The local receiver for UDP messages
 * @param [in] remote Address the remote IP address for the connection, must include a destination port.
 *
 * @retval non-null An allocated Io Operations structure for the connection
 * @retval null An error
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
MetisIoOperations *metisUdpTunnel_CreateOnListener(MetisForwarder *metis, MetisListenerOps *localListener, const CPIAddress *remoteAddress);

/**
 * Establishes a connection to a remote system over UDP
 *
 * The remoteAddress must be of the same type (i.e. v4 or v6) as the localAddress.  There must be an existing UDP listener
 * on the local address.  If either of these are not true, will return NULL.
 *
 * The connection will go in the table immediately, and will be in the "up" state.
 *
 * This function will lookup the appropraite listener, then use metisUdpTunnel_CreateOnListener().
 *
 * @param [in] metis An allocated MetisForwarder
 * @param [in] localAddress The local IP address and port to use for the connection
 * @param [in] remote Address the remote IP address for the connection, must include a destination port.
 *
 * @retval non-null An allocated Io Operations structure for the connection
 * @retval null An error
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
MetisIoOperations *metisUdpTunnel_Create(MetisForwarder *metis, const CPIAddress *localAddress, const CPIAddress *remoteAddress);

#endif // Metis_metis_UdpTunnel_h
