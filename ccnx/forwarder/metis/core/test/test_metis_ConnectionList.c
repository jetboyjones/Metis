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

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../metis_ConnectionList.c"
#include <parc/algol/parc_SafeMemory.h>
#include "testrig_MetisIoOperations.h"
#include <LongBow/unit-test.h>

LONGBOW_TEST_RUNNER(metis_ConnectionList)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(metis_ConnectionList)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(metis_ConnectionList)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, metisConnectionList_Append);
    LONGBOW_RUN_TEST_CASE(Global, metisConnectionList_Create_Destroy);

    LONGBOW_RUN_TEST_CASE(Global, metisConnectionList_Get);
    LONGBOW_RUN_TEST_CASE(Global, metisConnectionList_Length);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, metisConnectionList_Append)
{
    MetisIoOperations *ops = mockIoOperationsData_CreateSimple(1, 2, 3, true, true, true);
    MetisConnection *connection = metisConnection_Create(ops);

    MetisConnectionList *list = metisConnectionList_Create();
    metisConnectionList_Append(list, connection);
    metisConnection_Release(&connection);

    assertTrue(parcArrayList_Size(list->listOfConnections) == 1,
               "Got wrong list size, got %zu expected %u",
               parcArrayList_Size(list->listOfConnections), 1);

    metisConnectionList_Destroy(&list);
    mockIoOperationsData_Destroy(&ops);
}

LONGBOW_TEST_CASE(Global, metisConnectionList_Create_Destroy)
{
    MetisConnectionList *list = metisConnectionList_Create();
    assertNotNull(list, "Got null from Create");

    metisConnectionList_Destroy(&list);
    assertNull(list, "Destroy did not null the parameter");
}

LONGBOW_TEST_CASE(Global, metisConnectionList_Get)
{
    MetisIoOperations *ops = mockIoOperationsData_CreateSimple(1, 2, 3, true, true, true);
    MetisConnection *connection = metisConnection_Create(ops);

    MetisConnectionList *list = metisConnectionList_Create();
    metisConnectionList_Append(list, connection);

    MetisConnection *test = metisConnectionList_Get(list, 0);
    assertTrue(test == connection,
               "Got wrong connection, got %p expected %p",
               (void *) test, (void *) connection);

    metisConnection_Release(&connection);
    metisConnectionList_Destroy(&list);
    mockIoOperationsData_Destroy(&ops);
}

LONGBOW_TEST_CASE(Global, metisConnectionList_Length)
{
    MetisIoOperations *ops = mockIoOperationsData_CreateSimple(1, 2, 3, true, true, true);
    MetisConnection *connection = metisConnection_Create(ops);

    MetisConnectionList *list = metisConnectionList_Create();
    metisConnectionList_Append(list, connection);

    size_t length = metisConnectionList_Length(list);
    assertTrue(length == 1,
               "Got wrong list size, got %zu expected %u",
               length, 1);

    metisConnection_Release(&connection);
    metisConnectionList_Destroy(&list);
    mockIoOperationsData_Destroy(&ops);
}

LONGBOW_TEST_FIXTURE(Local)
{
    LONGBOW_RUN_TEST_CASE(Local, metisConnectionList_ArrayDestroyer);
}

LONGBOW_TEST_FIXTURE_SETUP(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Local, metisConnectionList_ArrayDestroyer)
{
    testUnimplemented("");
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(metis_ConnectionList);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}