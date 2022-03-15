/** \file */

#ifdef TEST

#include "unity.h"

#include "SMfsm.h"
#include "SMhndlr.h"
#include "mock_SMactivity.h"

/* `"` used intentionally.  This allows the user to override and provide his
 * own implementation before falling back to libc.
 */
#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"
#include "assert.h" /* For Sanity checks (Design by Contract) */

/** \brief Run before every test */
void setUp(void)
{
    return; /* Do nothing */
}

/** \brief Run after every test */
void tearDown(void)
{
    SMhndlr_setGrd0(false);

    return;
}

void test_SMfsm_assertNoNullPtrsOnInit(void)
{
    static stc_fsm_t stc_fsm;

    TEST_ASSERT_FAIL_ASSERT( SMfsm_init(NULL, &SMhndlr_statInit) );
    TEST_ASSERT_FAIL_ASSERT( SMfsm_init(&stc_fsm, NULL) );

    return;
}

void test_SMfsm_assertNoNullPtrOnSendEvt(void)
{
    TEST_ASSERT_FAIL_ASSERT( (void)SMfsm_sendEvt(NULL, 0u) );

    return;
}

void test_SMfsm_assertNoNullPtrOnGetStat(void)
{
    TEST_ASSERT_FAIL_ASSERT( (void)SMfsm_getStat(NULL) );

    return;
}

void test_SMfsm_assertNoNullPtrsOnTrans(void)
{
    static stc_fsm_t stc_fsm;

    TEST_ASSERT_FAIL_ASSERT( SMfsm_trans(NULL, &SMhndlr_statA) );
    TEST_ASSERT_FAIL_ASSERT( SMfsm_trans(&stc_fsm, NULL) );

    return;
}

void test_SMfsm_initAndQueryStat(void)
{
    const p_fn_sHndlr_t p_fn_stateExpected = &SMhndlr_statInit;
    p_fn_sHndlr_t p_fn_stateActual = NULL;

    /* Initialize FSM */
    static stc_fsm_t stc_fsm;
    SMfsm_init(&stc_fsm, p_fn_stateExpected);

    p_fn_stateActual = SMfsm_getStat(&stc_fsm);

    TEST_ASSERT_EQUAL_PTR(p_fn_stateExpected, p_fn_stateActual);

    return;
}

void test_SMfsm_ignoreUnknownEvt(void)
{
    const p_fn_sHndlr_t p_fn_stateExpected = &SMhndlr_statInit;
    p_fn_sHndlr_t p_fn_stateActual = NULL;

    bool b_evtHandledActual = true;

    SMactivity_ignore_Expect();

    /* Initialize FSM */
    static stc_fsm_t stc_fsm;
    SMfsm_init(&stc_fsm, &SMhndlr_statInit);

    b_evtHandledActual = SMfsm_sendEvt(&stc_fsm, en_sHndlr_evt_A);

    TEST_ASSERT_FALSE(b_evtHandledActual);

    p_fn_stateActual = SMfsm_getStat(&stc_fsm);

    TEST_ASSERT_EQUAL_PTR(p_fn_stateExpected, p_fn_stateActual);

    return;
}

void test_SMfsm_transActivitySequence(void)
{
    const p_fn_sHndlr_t p_fn_stateExpected = &SMhndlr_statB;
    p_fn_sHndlr_t p_fn_stateActual = NULL;

    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static stc_fsm_t stc_fsm;
    SMfsm_init(&stc_fsm, &SMhndlr_statA);

    SMfsm_trans(&stc_fsm, p_fn_stateExpected);

    p_fn_stateActual = SMfsm_getStat(&stc_fsm);

    TEST_ASSERT_EQUAL_PTR(p_fn_stateExpected, p_fn_stateActual);

    return;
}

void test_SMfsm_takeInitTrans(void)
{
    const p_fn_sHndlr_t p_fn_stateExpected = &SMhndlr_statA;
    p_fn_sHndlr_t p_fn_stateActual = NULL;

    bool b_evtHandledActual = false;

    SMactivity_entry_Expect();
    SMactivity_transInit_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM */
    static stc_fsm_t stc_fsm;
    SMfsm_init(&stc_fsm, &SMhndlr_statInit);

    b_evtHandledActual = SMfsm_sendEvt(&stc_fsm, en_fsm_evt_ENTRY);

    TEST_ASSERT_TRUE(b_evtHandledActual);

    p_fn_stateActual = SMfsm_getStat(&stc_fsm);

    TEST_ASSERT_EQUAL_PTR(p_fn_stateExpected, p_fn_stateActual);

    return;
}

void test_SMfsm_takeNormalTrans(void)
{
    const p_fn_sHndlr_t p_fn_stateExpected = &SMhndlr_statB;
    p_fn_sHndlr_t p_fn_stateActual = NULL;

    bool b_evtHandledActual = false;

    SMactivity_trans_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static stc_fsm_t stc_fsm;
    SMfsm_init(&stc_fsm, &SMhndlr_statA);

    b_evtHandledActual = SMfsm_sendEvt(&stc_fsm, en_sHndlr_evt_B);

    TEST_ASSERT_TRUE(b_evtHandledActual);

    p_fn_stateActual = SMfsm_getStat(&stc_fsm);

    TEST_ASSERT_EQUAL_PTR(p_fn_stateExpected, p_fn_stateActual);

    return;
}

void test_SMfsm_takeTransToSelf(void)
{
    const p_fn_sHndlr_t p_fn_stateExpected = &SMhndlr_statB;
    p_fn_sHndlr_t p_fn_stateActual = NULL;

    bool b_evtHandledActual = false;

    SMactivity_trans_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static stc_fsm_t stc_fsm;
    SMfsm_init(&stc_fsm, &SMhndlr_statB);

    b_evtHandledActual = SMfsm_sendEvt(&stc_fsm, en_sHndlr_evt_B);

    TEST_ASSERT_TRUE(b_evtHandledActual);

    p_fn_stateActual = SMfsm_getStat(&stc_fsm);

    TEST_ASSERT_EQUAL_PTR(p_fn_stateExpected, p_fn_stateActual);

    return;
}

void test_SMfsm_takeTrueGrd0TransViaExtdStatVar(void)
{
    const p_fn_sHndlr_t p_fn_stateExpected = &SMhndlr_statB;
    p_fn_sHndlr_t p_fn_stateActual = NULL;

    bool b_evtHandledActual = false;

    SMactivity_trans_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static stc_fsm_t stc_fsm;
    SMfsm_init(&stc_fsm, &SMhndlr_statC);
    SMhndlr_setGrd0(true);

    b_evtHandledActual = SMfsm_sendEvt(&stc_fsm, en_sHndlr_evt_B);

    TEST_ASSERT_TRUE(b_evtHandledActual);

    p_fn_stateActual = SMfsm_getStat(&stc_fsm);

    TEST_ASSERT_EQUAL_PTR(p_fn_stateExpected, p_fn_stateActual);

    return;
}

void test_SMfsm_takeTrueGrd1TransViaExtdStatVar(void)
{
    const p_fn_sHndlr_t p_fn_stateExpected = &SMhndlr_statZ;
    p_fn_sHndlr_t p_fn_stateActual = NULL;

    bool b_evtHandledActual = false;

    SMactivity_trans_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static stc_sHndlr_fsm_t stc_sHndlr_fsm;
    /* Explicit upcast needed (which is safe here) */
    SMfsm_init( (stc_fsm_t*)&stc_sHndlr_fsm, &SMhndlr_statZ );
    stc_sHndlr_fsm.b_grd1 = true;

    /* Explicit upcast needed (which is safe here) */
    b_evtHandledActual = SMfsm_sendEvt( (stc_fsm_t*)&stc_sHndlr_fsm,
                                        en_sHndlr_evt_Z );

    TEST_ASSERT_TRUE(b_evtHandledActual);

    /* Explicit upcast needed (which is safe here) */
    p_fn_stateActual = SMfsm_getStat( (stc_fsm_t*)&stc_sHndlr_fsm );

    TEST_ASSERT_EQUAL_PTR(p_fn_stateExpected, p_fn_stateActual);

    return;
}

void test_SMfsm_takeInternalTrans(void)
{
    const p_fn_sHndlr_t p_fn_stateExpected = &SMhndlr_statC;
    p_fn_sHndlr_t p_fn_stateActual = NULL;

    bool b_evtHandledActual = false;

    SMactivity_trans_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static stc_fsm_t stc_fsm;
    SMfsm_init(&stc_fsm, &SMhndlr_statC);

    b_evtHandledActual = SMfsm_sendEvt(&stc_fsm, en_sHndlr_evt_C);

    TEST_ASSERT_TRUE(b_evtHandledActual);

    p_fn_stateActual = SMfsm_getStat(&stc_fsm);

    TEST_ASSERT_EQUAL_PTR(p_fn_stateExpected, p_fn_stateActual);

    return;
}

#endif /* TEST */
