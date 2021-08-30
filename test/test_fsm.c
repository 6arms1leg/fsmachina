/** \file */

#ifdef TEST

#include "unity.h"

#include "fsm.h"
#include "stateHandler.h"
#include "mock_activity.h"

/* `"` used intentionally.  This allows the user to overwrite and provide his
 * own implementation before falling back to libc.
 */
#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"
#include "assert.h" /* For Sanity checks (Contract by Design) */

/** \brief Run before every test */
void setUp(void)
{
    return; /* Do nothing */
}

/** \brief Run after every test */
void tearDown(void)
{
    fn_sHndlr_setGrd0(false);

    return;
}

void test_fsm_assertNoNullPointersOnInitialization(void)
{
    static stc_fsm_t stc_fsm;

    TEST_ASSERT_FAIL_ASSERT( fn_fsm_ini(NULL, fn_sHndlr_stateIni) );
    TEST_ASSERT_FAIL_ASSERT( fn_fsm_ini(&stc_fsm, NULL) );

    return;
}

void test_fsm_assertNoNullPointerOnSendEvent(void)
{
    TEST_ASSERT_FAIL_ASSERT( (void)fn_fsm_sendEvt(NULL, (uint8_t)0U) );

    return;
}

void test_fsm_assertNoNullPointerOnGetState(void)
{
    TEST_ASSERT_FAIL_ASSERT( (void)fn_fsm_getState(NULL) );

    return;
}

void test_fsm_assertNoNullPointersOnTransition(void)
{
    static stc_fsm_t stc_fsm;

    TEST_ASSERT_FAIL_ASSERT( fn_fsm_tran(NULL, fn_sHndlr_stateA) );
    TEST_ASSERT_FAIL_ASSERT( fn_fsm_tran(&stc_fsm, NULL) );

    return;
}

void test_fsm_initializeAndQueryState(void)
{
    const p_fn_sHndlr_t stateExpected = fn_sHndlr_stateIni;
    p_fn_sHndlr_t stateActual = NULL;

    /* Initialize FSM */
    static stc_fsm_t stc_fsm;
    fn_fsm_ini(&stc_fsm, stateExpected);

    stateActual = fn_fsm_getState(&stc_fsm);

    TEST_ASSERT_EQUAL_PTR(stateExpected, stateActual);

    return;
}

void test_fsm_ignoreUnknownEvent(void)
{
    const p_fn_sHndlr_t stateExpected = fn_sHndlr_stateIni;
    p_fn_sHndlr_t stateActual = NULL;

    const bool evtHandledExpected = false;
    bool evtHandledActual = true;

    fn_activity_ignore_Expect();

    /* Initialize FSM */
    static stc_fsm_t stc_fsm;
    fn_fsm_ini(&stc_fsm, fn_sHndlr_stateIni);

    evtHandledActual = fn_fsm_sendEvt(&stc_fsm, en_sHndlr_evt_A);

    TEST_ASSERT_FALSE(evtHandledActual);

    stateActual = fn_fsm_getState(&stc_fsm);

    TEST_ASSERT_EQUAL_PTR(stateExpected, stateActual);

    return;
}

void test_fsm_transitionActivitySequence(void)
{
    const p_fn_sHndlr_t stateExpected = fn_sHndlr_stateB;
    p_fn_sHndlr_t stateActual = NULL;

    fn_activity_exit_Expect();
    fn_activity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static stc_fsm_t stc_fsm;
    fn_fsm_ini(&stc_fsm, fn_sHndlr_stateA);

    fn_fsm_tran(&stc_fsm, stateExpected);

    stateActual = fn_fsm_getState(&stc_fsm);

    TEST_ASSERT_EQUAL_PTR(stateExpected, stateActual);

    return;
}

void test_fsm_takeInitialTransition(void)
{
    const p_fn_sHndlr_t stateExpected = fn_sHndlr_stateA;
    p_fn_sHndlr_t stateActual = NULL;

    const bool evtHandledExpected = true;
    bool evtHandledActual = false;

    fn_activity_entry_Expect();
    fn_activity_tranIni_Expect();
    fn_activity_exit_Expect();
    fn_activity_entry_Expect();

    /* Initialize FSM */
    static stc_fsm_t stc_fsm;
    fn_fsm_ini(&stc_fsm, fn_sHndlr_stateIni);

    evtHandledActual = fn_fsm_sendEvt(&stc_fsm, en_fsm_evt_ENTRY);

    TEST_ASSERT_TRUE(evtHandledActual);

    stateActual = fn_fsm_getState(&stc_fsm);

    TEST_ASSERT_EQUAL_PTR(stateExpected, stateActual);

    return;
}

void test_fsm_takeNormalTransition(void)
{
    const p_fn_sHndlr_t stateExpected = fn_sHndlr_stateB;
    p_fn_sHndlr_t stateActual = NULL;

    const bool evtHandledExpected = true;
    bool evtHandledActual = false;

    fn_activity_tran_Expect();
    fn_activity_exit_Expect();
    fn_activity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static stc_fsm_t stc_fsm;
    fn_fsm_ini(&stc_fsm, fn_sHndlr_stateA);

    evtHandledActual = fn_fsm_sendEvt(&stc_fsm, en_sHndlr_evt_B);

    TEST_ASSERT_TRUE(evtHandledActual);

    stateActual = fn_fsm_getState(&stc_fsm);

    TEST_ASSERT_EQUAL_PTR(stateExpected, stateActual);

    return;
}

void test_fsm_takeTransitionToSelf(void)
{
    const p_fn_sHndlr_t stateExpected = fn_sHndlr_stateB;
    p_fn_sHndlr_t stateActual = NULL;

    const bool evtHandledExpected = true;
    bool evtHandledActual = false;

    fn_activity_tran_Expect();
    fn_activity_exit_Expect();
    fn_activity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static stc_fsm_t stc_fsm;
    fn_fsm_ini(&stc_fsm, fn_sHndlr_stateB);

    evtHandledActual = fn_fsm_sendEvt(&stc_fsm, en_sHndlr_evt_B);

    TEST_ASSERT_TRUE(evtHandledActual);

    stateActual = fn_fsm_getState(&stc_fsm);

    TEST_ASSERT_EQUAL_PTR(stateExpected, stateActual);

    return;
}

void test_fsm_takeTrueGuard0TransitionViaExtendedStateVariable(void)
{
    const p_fn_sHndlr_t stateExpected = fn_sHndlr_stateB;
    p_fn_sHndlr_t stateActual = NULL;

    const bool evtHandledExpected = true;
    bool evtHandledActual = false;

    fn_activity_tran_Expect();
    fn_activity_exit_Expect();
    fn_activity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static stc_fsm_t stc_fsm;
    fn_fsm_ini(&stc_fsm, fn_sHndlr_stateC);
    fn_sHndlr_setGrd0(true);

    evtHandledActual = fn_fsm_sendEvt(&stc_fsm, en_sHndlr_evt_B);

    TEST_ASSERT_TRUE(evtHandledActual);

    stateActual = fn_fsm_getState(&stc_fsm);

    TEST_ASSERT_EQUAL_PTR(stateExpected, stateActual);

    return;
}

void test_fsm_takeTrueGuard1TransitionViaExtendedStateVariable(void)
{
    const p_fn_sHndlr_t stateExpected = fn_sHndlr_stateZ;
    p_fn_sHndlr_t stateActual = NULL;

    const bool evtHandledExpected = true;
    bool evtHandledActual = false;

    fn_activity_tran_Expect();
    fn_activity_exit_Expect();
    fn_activity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static stc_sHndlr_fsm_t stc_sHndlr_fsm;
    /* Explicit upcast needed (which is safe here) */
    fn_fsm_ini( (stc_fsm_t*)&stc_sHndlr_fsm, fn_sHndlr_stateZ );
    stc_sHndlr_fsm.b_grd1 = true;

    /* Explicit upcast needed (which is safe here) */
    evtHandledActual = fn_fsm_sendEvt( (stc_fsm_t*)&stc_sHndlr_fsm,
                                       en_sHndlr_evt_Z );

    TEST_ASSERT_TRUE(evtHandledActual);

    /* Explicit upcast needed (which is safe here) */
    stateActual = fn_fsm_getState( (stc_fsm_t*)&stc_sHndlr_fsm );

    TEST_ASSERT_EQUAL_PTR(stateExpected, stateActual);

    return;
}

void test_fsm_takeInternalTransition(void)
{
    const p_fn_sHndlr_t stateExpected = fn_sHndlr_stateC;
    p_fn_sHndlr_t stateActual = NULL;

    const bool evtHandledExpected = true;
    bool evtHandledActual = false;

    fn_activity_tran_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static stc_fsm_t stc_fsm;
    fn_fsm_ini(&stc_fsm, fn_sHndlr_stateC);

    evtHandledActual = fn_fsm_sendEvt(&stc_fsm, en_sHndlr_evt_C);

    TEST_ASSERT_TRUE(evtHandledActual);

    stateActual = fn_fsm_getState(&stc_fsm);

    TEST_ASSERT_EQUAL_PTR(stateExpected, stateActual);

    return;
}

#endif /* TEST */