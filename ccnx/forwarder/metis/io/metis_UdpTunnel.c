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
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#include <config.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include <ccnx/forwarder/metis/io/metis_UdpTunnel.h>
#include <ccnx/forwarder/metis/io/metis_UdpConnection.h>

#include <LongBow/runtime.h>

MetisIoOperations *
metisUdpTunnel_CreateOnListener(MetisForwarder *metis, MetisListenerOps *localListener, const CPIAddress *remoteAddress)
{
    assertNotNull(metis, "Parameter metis must be non-null");
    assertNotNull(localListener, "Parameter localListener must be non-null");
    assertNotNull(remoteAddress, "Parameter remoteAddress must be non-null");

    MetisLogger *logger = metisForwarder_GetLogger(metis);

    MetisIoOperations *ops = NULL;
    if (localListener->getEncapType(localListener) == METIS_ENCAP_UDP) {
        const CPIAddress *localAddress = localListener->getListenAddress(localListener);
        CPIAddressType localType = cpiAddress_GetType(localAddress);
        CPIAddressType remoteType = cpiAddress_GetType(remoteAddress);

        if (localType == remoteType) {
            MetisAddressPair *pair = metisAddressPair_Create(localAddress, remoteAddress);
            bool isLocal = false;
            int fd = localListener->getSocket(localListener);
            ops = metisUdpConnection_Create(metis, fd, pair, isLocal);

            metisAddressPair_Release(&pair);
        } else {
            if (metisLogger_IsLoggable(logger, MetisLoggerFacility_IO, PARCLogLevel_Error)) {
                metisLogger_Log(logger, MetisLoggerFacility_IO, PARCLogLevel_Error, __func__,
                                "Local listener of type %s and remote type %s, cannot establish tunnel",
                                cpiAddress_TypeToString(localType),
                                cpiAddress_TypeToString(remoteType));
            }
        }
    } else {
        if (metisLogger_IsLoggable(logger, MetisLoggerFacility_IO, PARCLogLevel_Error)) {
            metisLogger_Log(logger, MetisLoggerFacility_IO, PARCLogLevel_Error, __func__,
                            "Local listener %p is not type UDP, cannot establish tunnel", (void *) localListener);
        }
    }

    return ops;
}

/*
 * wrapper for metisUdpTunnel_CreateOnListener.  Lookup to see if we have a listener on the local address.
 * If so, call metisUdpTunnel_CreateOnListener, otherwise return NULL
 */
MetisIoOperations *
metisUdpTunnel_Create(MetisForwarder *metis, const CPIAddress *localAddress, const CPIAddress *remoteAddress)
{
    MetisListenerSet *set = metisForwarder_GetListenerSet(metis);
    MetisListenerOps *listener = metisListenerSet_Find(set, METIS_ENCAP_UDP, localAddress);
    MetisIoOperations *ops = NULL;
    if (listener) {
        ops = metisUdpTunnel_CreateOnListener(metis, listener, remoteAddress);
    } else {
        if (metisLogger_IsLoggable(metisForwarder_GetLogger(metis), MetisLoggerFacility_IO, PARCLogLevel_Error)) {
            char *str = cpiAddress_ToString(localAddress);
            metisLogger_Log(metisForwarder_GetLogger(metis), MetisLoggerFacility_IO, PARCLogLevel_Error, __func__,
                            "Could not find listener to match address %s", str);
            parcMemory_Deallocate((void **) &str);
        }
    }
    return ops;
}

