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
    static SMfsm_fsm_t fsm;

    TEST_ASSERT_FAIL_ASSERT( SMfsm_init(NULL, &SMhndlr_statInit) );
    TEST_ASSERT_FAIL_ASSERT( SMfsm_init(&fsm, NULL) );

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
    static SMfsm_fsm_t fsm;

    TEST_ASSERT_FAIL_ASSERT( SMfsm_trans(NULL, &SMhndlr_statA) );
    TEST_ASSERT_FAIL_ASSERT( SMfsm_trans(&fsm, NULL) );

    return;
}

void test_SMfsm_initAndQueryStat(void)
{
    const SMfsm_p_hndlr_t p_statHndlrExp = &SMhndlr_statInit;
    SMfsm_p_hndlr_t p_statHndlrAct = NULL;

    /* Initialize FSM */
    static SMfsm_fsm_t fsm;
    SMfsm_init(&fsm, p_statHndlrExp);

    p_statHndlrAct = SMfsm_getStat(&fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);

    return;
}

void test_SMfsm_ignoreUnknownEvt(void)
{
    const SMfsm_p_hndlr_t p_statHndlrExp = &SMhndlr_statInit;
    SMfsm_p_hndlr_t p_statHndlrAct = NULL;

    bool evtHandled = true;

    SMactivity_ignore_Expect();

    /* Initialize FSM */
    static SMfsm_fsm_t fsm;
    SMfsm_init(&fsm, &SMhndlr_statInit);

    evtHandled = SMfsm_sendEvt(&fsm, SMHNDLR_EVT_A);

    TEST_ASSERT_FALSE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(&fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);

    return;
}

void test_SMfsm_transActivitySequence(void)
{
    const SMfsm_p_hndlr_t p_statHndlrExp = &SMhndlr_statB;
    SMfsm_p_hndlr_t p_statHndlrAct = NULL;

    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static SMfsm_fsm_t fsm;
    SMfsm_init(&fsm, &SMhndlr_statA);

    SMfsm_trans(&fsm, p_statHndlrExp);

    p_statHndlrAct = SMfsm_getStat(&fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);

    return;
}

void test_SMfsm_takeInitTrans(void)
{
    const SMfsm_p_hndlr_t p_statHndlrExp = &SMhndlr_statA;
    SMfsm_p_hndlr_t p_statHndlrAct = NULL;

    bool evtHandled = false;

    SMactivity_entry_Expect();
    SMactivity_transInit_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM */
    static SMfsm_fsm_t fsm;
    SMfsm_init(&fsm, &SMhndlr_statInit);

    evtHandled = SMfsm_sendEvt(&fsm, SMFSM_ENTRY);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(&fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);

    return;
}

void test_SMfsm_takeNormalTrans(void)
{
    const SMfsm_p_hndlr_t p_statHndlrExp = &SMhndlr_statB;
    SMfsm_p_hndlr_t p_statHndlrAct = NULL;

    bool evtHandled = false;

    SMactivity_trans_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static SMfsm_fsm_t fsm;
    SMfsm_init(&fsm, &SMhndlr_statA);

    evtHandled = SMfsm_sendEvt(&fsm, SMHNDLR_EVT_B);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(&fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);

    return;
}

void test_SMfsm_takeTransToSelf(void)
{
    const SMfsm_p_hndlr_t p_statHndlrExp = &SMhndlr_statB;
    SMfsm_p_hndlr_t p_statHndlrAct = NULL;

    bool evtHandled = false;

    SMactivity_trans_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static SMfsm_fsm_t fsm;
    SMfsm_init(&fsm, &SMhndlr_statB);

    evtHandled = SMfsm_sendEvt(&fsm, SMHNDLR_EVT_B);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(&fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);

    return;
}

void test_SMfsm_takeTrueGrd0TransViaExtdStatVar(void)
{
    const SMfsm_p_hndlr_t p_statHndlrExp = &SMhndlr_statB;
    SMfsm_p_hndlr_t p_statHndlrAct = NULL;

    bool evtHandled = false;

    SMactivity_trans_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static SMfsm_fsm_t fsm;
    SMfsm_init(&fsm, &SMhndlr_statC);
    SMhndlr_setGrd0(true);

    evtHandled = SMfsm_sendEvt(&fsm, SMHNDLR_EVT_B);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(&fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);

    return;
}

void test_SMfsm_takeTrueGrd1TransViaExtdStatVar(void)
{
    const SMfsm_p_hndlr_t p_statHndlrExp = &SMhndlr_statZ;
    SMfsm_p_hndlr_t p_statHndlrAct = NULL;

    bool evtHandled = false;

    SMactivity_trans_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static SMhndlr_fsm_t fsm;
    /* Explicit upcast needed (which is safe here) */
    SMfsm_init( (SMfsm_fsm_t*)&fsm, &SMhndlr_statZ );
    fsm.grd1 = true;

    /* Explicit upcast needed (which is safe here) */
    evtHandled = SMfsm_sendEvt( (SMfsm_fsm_t*)&fsm, SMHNDLR_EVT_Z );

    TEST_ASSERT_TRUE(evtHandled);

    /* Explicit upcast needed (which is safe here) */
    p_statHndlrAct = SMfsm_getStat( (SMfsm_fsm_t*)&fsm );

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);

    return;
}

void test_SMfsm_takeInternalTrans(void)
{
    const SMfsm_p_hndlr_t p_statHndlrExp = &SMhndlr_statC;
    SMfsm_p_hndlr_t p_statHndlrAct = NULL;

    bool evtHandled = false;

    SMactivity_trans_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
     * deviation from UML SM diagram where initial transition activity is
     * executed first)
     */
    static SMfsm_fsm_t fsm;
    SMfsm_init(&fsm, &SMhndlr_statC);

    evtHandled = SMfsm_sendEvt(&fsm, SMHNDLR_EVT_C);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(&fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);

    return;
}

#endif /* TEST */
