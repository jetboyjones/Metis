/*
 * Copyright (c) 2014-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @file metis_TlvOps.h
 * @brief The API for TLV schemas
 *
 * Each TLV schema must implement this API
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2014-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#ifndef Metis_metis_TlvOps_h
#define Metis_metis_TlvOps_h

#include <stdbool.h>
#include <parc/algol/parc_Buffer.h>
#include <parc/security/parc_CryptoHash.h>
#include <ccnx/forwarder/metis/tlv/metis_TlvSkeleton.h>
#include <ccnx/api/control/cpi_ControlMessage.h>

typedef struct metis_tlv_ops {
    /**
     * Fills in the packet TLV skeleton
     *
     * The skeleton must have been initialized with the correct parser and packet buffer.
     *
     * @param [in] skeleton An allocated MetisTlvSkeleton to fill in
     *
     * @retval true Good parse
     * @retval false Error
     *
     * Example:
     * @code
     * <#example#>
     * @endcode
     */
    bool (*parse)(MetisTlvSkeleton *skeleton);

    /**
     * Computes the ContentObjectHash over a packet
     *
     * <#Paragraphs Of Explanation#>
     *
     * @param [in] packet Packet memory, pointing to byte 0 of the fixed header
     *
     * @return non-null The sha256 hash
     * @return null An error (or not a content object)
     *
     * Example:
     * @code
     * {
     *     <#example#>
     * }
     * @endcode
     */
    PARCCryptoHash * (*computeContentObjectHash)(const uint8_t *packet);

    /**
     * @function metisTlv_EncodeCPI
     * @abstract Encodes a CPI control message in TLV format
     * @discussion
     *   <#Discussion#>
     *
     * @param <#param1#>
     * @return An allocated message, must call <code>metisMessage_Destroy()</code> on it.
     */
    PARCBuffer *(*encodeControlPlaneInformation)(const CCNxControl *cpiControlMessage);

    /**
     * Returns the total header length based on the Fixed Header
     *
     * The length may be 0 for an unsupported FixedHeader version or other error.
     *
     * @param [in] packet Packet memory pointing to byte 0 of the Fixed Header
     *
     * @retval number Total header length
     *
     * Example:
     * @code
     * <#example#>
     * @endcode
     */
    size_t (*totalHeaderLength)(const uint8_t *packet);

    /**
     * Returns the total packet length based on the Fixed Header
     *
     * The length may be 0 for an unsupported FixedHeader version or other error.
     *
     * @param [in] packet Packet memory pointing to byte 0 of the Fixed Header
     *
     * @retval number Total packet length
     *
     * Example:
     * @code
     * <#example#>
     * @endcode
     */
    size_t (*totalPacketLength)(const uint8_t *packet);

    /**
     * Returns the length of the fixed header
     *
     * The length may be 0 for an unsupported FixedHeader version or other error.
     *
     * @param [in] packet Packet memory pointing to byte 0 of the Fixed Header
     *
     * @retval number Total packet length
     *
     * Example:
     * @code
     * <#example#>
     * @endcode
     */
    size_t (*fixedHeaderLength)(const uint8_t *packet);

    /**
     * Determines if the FixedHeader PacketType is Intereest
     *
     * <#Paragraphs Of Explanation#>
     *
     * @param [in] packet Packet memory, pointing to byte 0 of fixed header
     *
     * @retval true PacketType is Interest
     * @retval false PacketType is not Interest
     *
     * Example:
     * @code
     * <#example#>
     * @endcode
     */
    bool (*isPacketTypeInterest)(const uint8_t *packet);

    /**
     * Determines if the FixedHeader PacketType is ContentObject
     *
     * <#Paragraphs Of Explanation#>
     *
     * @param [in] packet Packet memory, pointing to byte 0 of fixed header
     *
     * @retval true PacketType is ContentObject
     * @retval false PacketType is not ContentObject
     *
     * Example:
     * @code
     * <#example#>
     * @endcode
     */
    bool (*isPacketTypeContentObject)(const uint8_t *packet);

    /**
     * Determines if the FixedHeader PacketType is InterestReturn
     *
     * <#Paragraphs Of Explanation#>
     *
     * @param [in] packet Packet memory, pointing to byte 0 of fixed header
     *
     * @retval true PacketType is InterestReturn
     * @retval false PacketType is not InterestReturn
     *
     * Example:
     * @code
     * <#example#>
     * @endcode
     */
    bool (*isPacketTypeInterestReturn)(const uint8_t *packet);

    /**
     * Determines if the FixedHeader PacketType is Control
     *
     * <#Paragraphs Of Explanation#>
     *
     * @param [in] packet Packet memory, pointing to byte 0 of fixed header
     *
     * @retval true PacketType is Control
     * @retval false PacketType is not Control
     *
     * Example:
     * @code
     * <#example#>
     * @endcode
     */
    bool (*isPacketTypeControl)(const uint8_t *packet);

    /**
     * Determines if the FixedHeader PacketType is Hop By Hop Fragment
     *
     * <#Paragraphs Of Explanation#>
     *
     * @param [in] packet Packet memory, pointing to byte 0 of fixed header
     *
     * @retval true PacketType is Hop By Hop Fragment
     * @retval false PacketType is not Hop By Hop Fragment
     *
     * Example:
     * @code
     * <#example#>
     * @endcode
     */
    bool (*isPacketTypeHopByHopFragment)(const uint8_t *packet);
} MetisTlvOps;


#endif // Metis_metis_TlvOps_h
