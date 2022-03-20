/** \file */

#ifdef TEST

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "assert.h" /* Sanity checks (Design by Contract); replaced for unit
                       testing */

#include "unity.h"

#include "SMfsm.h"

#include "SMstatHndlr.h"
#include "mock_SMactivity.h"

/** \brief Run before every test */
void setUp(void) {
    SMstatHndlr_fsmCtor(); /* Init. FSM object */
}

/** \brief Run after every test */
void tearDown(void) {
    SMstatHndlr_setGrd0(false); /* Reset extended state variable */
}

void test_SMfsm_assertNoNullPtrsOnInit(void) {
    TEST_ASSERT_FAIL_ASSERT(SMfsm_init(NULL, &SMstatHndlr_statA));
    TEST_ASSERT_FAIL_ASSERT(SMfsm_init(SMstatHndlr_p_fsm, NULL));
        /* Attempt to incorrectly re-init. FSM */
}

void test_SMfsm_assertNoNullPtrOnSendEvt(void) {
    TEST_ASSERT_FAIL_ASSERT((void)SMfsm_sendEvt(NULL, 0u));
}

void test_SMfsm_assertNoNullPtrOnGetStat(void) {
    TEST_ASSERT_FAIL_ASSERT((void)SMfsm_getStat(NULL));
}

void test_SMfsm_assertNoNullPtrsOnTrans(void) {
    TEST_ASSERT_FAIL_ASSERT(SMfsm_trans(NULL, &SMstatHndlr_statA));
    TEST_ASSERT_FAIL_ASSERT(SMfsm_trans(SMstatHndlr_p_fsm, NULL));
}

void test_SMfsm_initAndQueryStat(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statA;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;

    /* Re-init. FSM (directly to testing state to reduce redundancy in tests;
       UML SM diagram deviation where init. trans. activity is exec. first) */
    SMfsm_init(SMstatHndlr_p_fsm, p_statHndlrExp);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

void test_SMfsm_ignoreUnknownEvt(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statA;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = true;

    SMactivity_ignore_Expect();

    /* Re-init. FSM (directly to testing state to reduce redundancy in tests;
       UML SM diagram deviation where init. trans. activity is exec. first) */
    SMfsm_init(SMstatHndlr_p_fsm, p_statHndlrExp);

    evtHandled = SMfsm_sendEvt(SMstatHndlr_p_fsm, SMHNDLR_EVT_C);

    TEST_ASSERT_FALSE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

void test_SMfsm_transActivitySequence(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statB;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;

    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Re-init. FSM (directly to testing state to reduce redundancy in tests;
       UML SM diagram deviation where init. trans. activity is exec. first) */
    SMfsm_init(SMstatHndlr_p_fsm, &SMstatHndlr_statA);

    SMfsm_trans(SMstatHndlr_p_fsm, p_statHndlrExp);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

void test_SMfsm_takeInitTrans(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statA;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_entry_Expect();
    SMactivity_transInit_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    evtHandled = SMfsm_sendEvt(SMstatHndlr_p_fsm, SMFSM_ENTRY);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

void test_SMfsm_takeNormalTrans(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statB;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_trans_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Re-init. FSM (directly to testing state to reduce redundancy in tests;
       UML SM diagram deviation where init. trans. activity is exec. first) */
    SMfsm_init(SMstatHndlr_p_fsm, &SMstatHndlr_statA);

    evtHandled = SMfsm_sendEvt(SMstatHndlr_p_fsm, SMHNDLR_EVT_B);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

void test_SMfsm_takeTransToSelf(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statB;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_trans_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Re-init. FSM (directly to testing state to reduce redundancy in tests;
       UML SM diagram deviation where init. trans. activity is exec. first) */
    SMfsm_init(SMstatHndlr_p_fsm, p_statHndlrExp);

    evtHandled = SMfsm_sendEvt(SMstatHndlr_p_fsm, SMHNDLR_EVT_B);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

void test_SMfsm_takeTrueGrd0TransViaExtdStatVar(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statB;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_trans_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Re-init. FSM (directly to testing state to reduce redundancy in tests;
       UML SM diagram deviation where init. trans. activity is exec. first) */
    SMfsm_init(SMstatHndlr_p_fsm, &SMstatHndlr_statC);
    SMstatHndlr_setGrd0(true); /* Set extended state variable */

    evtHandled = SMfsm_sendEvt(SMstatHndlr_p_fsm, SMHNDLR_EVT_B);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

void test_SMfsm_takeTrueGrd1TransViaExtdStatVar(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statZ;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_trans_Expect();
    SMactivity_exit_Expect();
    SMactivity_entry_Expect();

    /* Init. derived FSM (directly to testing state to reduce redundancy in
       tests; UML SM diagram deviation where init. trans. activity is exec.
       first) */
    SMstatHndlr_fsm_t fsm;
    SMfsm_init((SMfsm_fsm_t*)&fsm, p_statHndlrExp);
        /* Explicit type upcast needed (safe here) */
    fsm.grd1 = true; /* Set extended state variable */

    evtHandled = SMfsm_sendEvt((SMfsm_fsm_t*)&fsm, SMHNDLR_EVT_Z);
        /* Explicit type upcast needed (safe here) */

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat((SMfsm_fsm_t*)&fsm);
        /* Explicit type upcast needed (safe here) */

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

void test_SMfsm_takeInternalTrans(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statC;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_trans_Expect();

    /* Re-init. FSM (directly to testing state to reduce redundancy in tests;
       UML SM diagram deviation where init. trans. activity is exec. first) */
    SMfsm_init(SMstatHndlr_p_fsm, p_statHndlrExp);

    evtHandled = SMfsm_sendEvt(SMstatHndlr_p_fsm, SMHNDLR_EVT_C);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

#endif /* TEST */
