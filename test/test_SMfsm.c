/** \file */

#ifdef TEST

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "assert.h" /* Sanity checks (Design by Contract); replaced for unit
                       testing */

#include "unity.h"

#include "SMfsm.h"

#include "SMhndlr.h"
#include "mock_SMactivity.h"

/** \brief Run before every test */
void setUp(void) {
    /* Do nothing */
}

/** \brief Run after every test */
void tearDown(void) {
    SMhndlr_setGrd0(false); /* Reset extended state variable */
}

void test_SMfsm_assertNoNullPtrsOnInit(void) {
    SMfsm_fsm_t fsm;

    TEST_ASSERT_FAIL_ASSERT(SMfsm_init(NULL, &SMhndlr_statInit));
    TEST_ASSERT_FAIL_ASSERT(SMfsm_init(&fsm, NULL));
}

void test_SMfsm_assertNoNullPtrOnSendEvt(void) {
    TEST_ASSERT_FAIL_ASSERT((void)SMfsm_sendEvt(NULL, 0u));
}

void test_SMfsm_assertNoNullPtrOnGetStat(void) {
    TEST_ASSERT_FAIL_ASSERT((void)SMfsm_getStat(NULL));
}

void test_SMfsm_assertNoNullPtrsOnTrans(void) {
    SMfsm_fsm_t fsm;

    TEST_ASSERT_FAIL_ASSERT(SMfsm_trans(NULL, &SMhndlr_statA));
    TEST_ASSERT_FAIL_ASSERT(SMfsm_trans(&fsm, NULL));
}

void test_SMfsm_initAndQueryStat(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMhndlr_statInit;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;

    /* Initialize FSM */
    SMfsm_fsm_t fsm;
    SMfsm_init(&fsm, p_statHndlrExp);

    p_statHndlrAct = SMfsm_getStat(&fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

void test_SMfsm_ignoreUnknownEvt(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMhndlr_statInit;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = true;

    SMactivity_ignore_Expect();

    /* Initialize FSM */
    SMfsm_fsm_t fsm;
    SMfsm_init(&fsm, &SMhndlr_statInit);

    evtHandled = SMfsm_sendEvt(&fsm, SMHNDLR_EVT_A);

    TEST_ASSERT_FALSE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(&fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

void test_SMfsm_transActivitySequence(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMhndlr_statB;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;

    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
       deviation from UML SM diagram where initial transition activity is
       executed first) */
    SMfsm_fsm_t fsm;
    SMfsm_init(&fsm, &SMhndlr_statA);

    SMfsm_trans(&fsm, p_statHndlrExp);

    p_statHndlrAct = SMfsm_getStat(&fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

void test_SMfsm_takeInitTrans(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMhndlr_statA;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_entry_Expect();
    SMactivity_transInit_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM */
    SMfsm_fsm_t fsm;
    SMfsm_init(&fsm, &SMhndlr_statInit);

    evtHandled = SMfsm_sendEvt(&fsm, SMFSM_ENTRY);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(&fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

void test_SMfsm_takeNormalTrans(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMhndlr_statB;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_trans_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
       deviation from UML SM diagram where initial transition activity is
       executed first) */
    SMfsm_fsm_t fsm;
    SMfsm_init(&fsm, &SMhndlr_statA);

    evtHandled = SMfsm_sendEvt(&fsm, SMHNDLR_EVT_B);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(&fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

void test_SMfsm_takeTransToSelf(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMhndlr_statB;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_trans_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
       deviation from UML SM diagram where initial transition activity is
       executed first) */
    SMfsm_fsm_t fsm;
    SMfsm_init(&fsm, &SMhndlr_statB);

    evtHandled = SMfsm_sendEvt(&fsm, SMHNDLR_EVT_B);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(&fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

void test_SMfsm_takeTrueGrd0TransViaExtdStatVar(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMhndlr_statB;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_trans_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
       deviation from UML SM diagram where initial transition activity is
       executed first) */
    SMfsm_fsm_t fsm;
    SMfsm_init(&fsm, &SMhndlr_statC);
    SMhndlr_setGrd0(true);

    evtHandled = SMfsm_sendEvt(&fsm, SMHNDLR_EVT_B);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(&fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

void test_SMfsm_takeTrueGrd1TransViaExtdStatVar(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMhndlr_statZ;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_trans_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
       deviation from UML SM diagram where initial transition activity is
       executed first) */
    SMhndlr_fsm_t fsm;
    SMfsm_init((SMfsm_fsm_t*)&fsm, &SMhndlr_statZ);
        /* Explicit type upcast needed (safe here) */
    fsm.grd1 = true;

    evtHandled = SMfsm_sendEvt((SMfsm_fsm_t*)&fsm, SMHNDLR_EVT_Z);
        /* Explicit type upcast needed (safe here) */

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat((SMfsm_fsm_t*)&fsm);
        /* Explicit type upcast needed (safe here) */

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

void test_SMfsm_takeInternalTrans(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMhndlr_statC;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_trans_Expect();

    /* Initialize FSM (directly to testing state to reduce redundancy in tests;
       deviation from UML SM diagram where initial transition activity is
       executed first) */
    SMfsm_fsm_t fsm;
    SMfsm_init(&fsm, &SMhndlr_statC);

    evtHandled = SMfsm_sendEvt(&fsm, SMHNDLR_EVT_C);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(&fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

#endif /* TEST */
