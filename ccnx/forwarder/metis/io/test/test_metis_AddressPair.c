/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../metis_AddressPair.c"
#include <LongBow/unit-test.h>
#include <parc/algol/parc_SafeMemory.h>

LONGBOW_TEST_RUNNER(metis_AddressPair)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(metis_AddressPair)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(metis_AddressPair)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, metisAddressPair_Create_Destroy);
    LONGBOW_RUN_TEST_CASE(Global, metisAddressPair_Equals);
    LONGBOW_RUN_TEST_CASE(Global, metisAddressPair_Equals_NotEqual);
    LONGBOW_RUN_TEST_CASE(Global, metisAddressPair_EqualsAddresses);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, metisAddressPair_Create_Destroy)
{
    CPIAddress *a = cpiAddress_CreateFromInterface(1);
    CPIAddress *b = cpiAddress_CreateFromInterface(2);

    size_t allocbase = parcSafeMemory_Outstanding();
    MetisAddressPair *pair = metisAddressPair_Create(a, b);
    metisAddressPair_Release(&pair);

    assertTrue(parcSafeMemory_Outstanding() == allocbase,
               "Memory out of balance, expected %zu got %u",
               allocbase,
               parcSafeMemory_Outstanding());

    cpiAddress_Destroy(&a);
    cpiAddress_Destroy(&b);
}

LONGBOW_TEST_CASE(Global, metisAddressPair_Equals)
{
    CPIAddress *a = cpiAddress_CreateFromInterface(1);
    CPIAddress *b = cpiAddress_CreateFromInterface(2);

    MetisAddressPair *pair_a = metisAddressPair_Create(a, b);
    MetisAddressPair *pair_b = metisAddressPair_Create(a, b);

    assertTrue(metisAddressPair_Equals(pair_a, pair_b), "Two equal address pairs did not compare equal");

    metisAddressPair_Release(&pair_a);
    metisAddressPair_Release(&pair_b);

    cpiAddress_Destroy(&a);
    cpiAddress_Destroy(&b);
}

LONGBOW_TEST_CASE(Global, metisAddressPair_Equals_NotEqual)
{
    CPIAddress *a = cpiAddress_CreateFromInterface(1);
    CPIAddress *b = cpiAddress_CreateFromInterface(2);

    MetisAddressPair *pair_a = metisAddressPair_Create(a, b);
    MetisAddressPair *pair_b = metisAddressPair_Create(b, a);

    assertFalse(metisAddressPair_Equals(pair_a, pair_b), "Two unequal address pairs compared equal");

    metisAddressPair_Release(&pair_a);
    metisAddressPair_Release(&pair_b);
    cpiAddress_Destroy(&a);
    cpiAddress_Destroy(&b);
}

LONGBOW_TEST_CASE(Global, metisAddressPair_EqualsAddresses)
{
    CPIAddress *a = cpiAddress_CreateFromInterface(1);
    CPIAddress *b = cpiAddress_CreateFromInterface(2);

    MetisAddressPair *pair_a = metisAddressPair_Create(a, b);

    assertTrue(metisAddressPair_EqualsAddresses(pair_a, a, b), "Two equal address pairs did not compare equal");

    metisAddressPair_Release(&pair_a);
    cpiAddress_Destroy(&a);
    cpiAddress_Destroy(&b);
}

LONGBOW_TEST_FIXTURE(Local)
{
}

LONGBOW_TEST_FIXTURE_SETUP(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Local)
{
    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(metis_AddressPair);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
