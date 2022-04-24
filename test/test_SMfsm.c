/** \file */

#ifdef TEST

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "assert.h" /* Sanity checks (Design by Contract); replaced for unit
                       testing */

#include "unity.h"

#include "SMfsm.h"

#include "mock_SMactivity.h"

#include "SMstatHndlr.h"

/* OPERATIONS
 * ==========
 */

/** \brief Run before every test */
void setUp(void) {
    SMstatHndlr_fsmCtor(); /* Init. FSM object */
}

/** \brief Run after every test */
void tearDown(void) {
    SMstatHndlr_setGrd0(false); /* Reset extended state variable */
}

/**
 * \brief Test that assert fires on attempt to init. FSM object with `NULL`
 * pointers
 */
void test_SMfsm_assertNoNullPtrOnInit(void) {
    TEST_ASSERT_FAIL_ASSERT(SMfsm_init(NULL, &SMstatHndlr_statA));
    TEST_ASSERT_FAIL_ASSERT(SMfsm_init(SMstatHndlr_p_fsm, NULL));
        /* Attempt to incorrectly re-init. FSM */
}

/**
 * \brief Test that assert fires on attempt to send event to `NULL` pointer FSM
 * object or non-init. (non-`NULL` pointer) FSM object
 */
void test_SMfsm_assertNoNullPtrOnSendEvt(void) {
    SMfsm_fsm_t fsm = {0}; /* No init. (`fsm->p_statHndlr == NULL`) */

    TEST_ASSERT_FAIL_ASSERT((void)SMfsm_sendEvt(NULL, 0u));
    TEST_ASSERT_FAIL_ASSERT((void)SMfsm_sendEvt(&fsm, 0u));
}

/**
 * \brief Test that assert fires on attempt to query state of `NULL` pointer
 * FSM object
 */
void test_SMfsm_assertNoNullPtrOnGetStat(void) {
    TEST_ASSERT_FAIL_ASSERT((void)SMfsm_getStat(NULL));
}

/**
 * \brief Test that assert fires on attempt to perform state trans. on `NULL`
 * pointer FSM object, non-init. (non-`NULL` pointer) FSM object or trans. to
 * `NULL` pointer state handler
 */
void test_SMfsm_assertNoNullPtrOnTrans(void) {
    SMfsm_fsm_t fsm = {0}; /* No init. (`fsm->p_statHndlr == NULL`) */

    TEST_ASSERT_FAIL_ASSERT(SMfsm_trans(NULL, &SMstatHndlr_statA));
    TEST_ASSERT_FAIL_ASSERT(SMfsm_trans(&fsm, &SMstatHndlr_statA));
    TEST_ASSERT_FAIL_ASSERT(SMfsm_trans(SMstatHndlr_p_fsm, NULL));
}

/** \brief Test if FSM is init. to defined state correctly */
void test_SMfsm_initAndQueryStat(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statA;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;

    /* Re-init. FSM (directly to testing state to reduce redundancy in tests;
       UML SM diagram deviation where init. trans. activity is exec. first) */
    SMfsm_init(SMstatHndlr_p_fsm, p_statHndlrExp);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

/** \brief Test if unknown events are ignored correctly */
void test_SMfsm_ignoreUnknownEvt(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statA;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = true;

    SMactivity_ignore_Expect();

    /* Re-init. FSM (directly to testing state to reduce redundancy in tests;
       UML SM diagram deviation where init. trans. activity is exec. first) */
    SMfsm_init(SMstatHndlr_p_fsm, p_statHndlrExp);

    evtHandled = SMfsm_sendEvt(SMstatHndlr_p_fsm, SMSTATHNDLR_EVT_C);

    TEST_ASSERT_FALSE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

/** \brief Test if state trans. activity sequence (order) is exec. correctly */
void test_SMfsm_transActivitySequence(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statB;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;

    SMactivity_exit_Expect('A');
    SMactivity_entry_Expect('B');

    /* Re-init. FSM (directly to testing state to reduce redundancy in tests;
       UML SM diagram deviation where init. trans. activity is exec. first) */
    SMfsm_init(SMstatHndlr_p_fsm, &SMstatHndlr_statA);

    SMfsm_trans(SMstatHndlr_p_fsm, p_statHndlrExp);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

/** \brief Test if init. state trans. is taken correctly */
void test_SMfsm_takeInitTrans(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statA;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_entry_Expect('0');
    SMactivity_trans_Expect('0', 'A');
    SMactivity_exit_Expect('0');
    SMactivity_entry_Expect('A');

    evtHandled = SMfsm_sendEvt(SMstatHndlr_p_fsm, SMFSM_ENTRY);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

/** \brief Test if "normal" state trans. is taken correctly */
void test_SMfsm_takeNormalTrans(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statB;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_trans_Expect('A', 'B');
    SMactivity_exit_Expect('A');
    SMactivity_entry_Expect('B');

    /* Re-init. FSM (directly to testing state to reduce redundancy in tests;
       UML SM diagram deviation where init. trans. activity is exec. first) */
    SMfsm_init(SMstatHndlr_p_fsm, &SMstatHndlr_statA);

    evtHandled = SMfsm_sendEvt(SMstatHndlr_p_fsm, SMSTATHNDLR_EVT_B);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

/** \brief Test if state "trans. to self" is taken correctly */
void test_SMfsm_takeTransToSelf(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statB;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_trans_Expect('B', 'B');
    SMactivity_exit_Expect('B');
    SMactivity_entry_Expect('B');

    /* Re-init. FSM (directly to testing state to reduce redundancy in tests;
       UML SM diagram deviation where init. trans. activity is exec. first) */
    SMfsm_init(SMstatHndlr_p_fsm, p_statHndlrExp);

    evtHandled = SMfsm_sendEvt(SMstatHndlr_p_fsm, SMSTATHNDLR_EVT_B);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

/**
 * \brief Test if guarded state trans. (implemented via private extended state
 * variable) is taken correctly
 */
void test_SMfsm_takeTrueGrd0TransViaExtdStatVar(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statB;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_trans_Expect('C', 'B');
    SMactivity_exit_Expect('C');
    SMactivity_entry_Expect('B');

    /* Re-init. FSM (directly to testing state to reduce redundancy in tests;
       UML SM diagram deviation where init. trans. activity is exec. first) */
    SMfsm_init(SMstatHndlr_p_fsm, &SMstatHndlr_statC);
    SMstatHndlr_setGrd0(true); /* Set extended state variable */

    evtHandled = SMfsm_sendEvt(SMstatHndlr_p_fsm, SMSTATHNDLR_EVT_B);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

/**
 * \brief Test if guarded state trans. (implemented via extended state
 * variable inside derived FSM object) is taken correctly
 */
void test_SMfsm_takeTrueGrd1TransViaExtdStatVar(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statZ;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_trans_Expect('Z', 'Z');
    SMactivity_exit_Expect('Z');
    SMactivity_entry_Expect('Z');

    SMstatHndlr_fsmZCtor(); /* Init. derived FSM object */

    evtHandled = SMfsm_sendEvt(SMstatHndlr_p_fsmZ, SMSTATHNDLR_EVT_Z);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsmZ);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

/** \brief Test if "internal" state trans. is taken correctly */
void test_SMfsm_takeInternalTrans(void) {
    const SMfsm_p_statHndlr_t p_statHndlrExp = &SMstatHndlr_statC;
    SMfsm_p_statHndlr_t p_statHndlrAct = NULL;
    bool evtHandled = false;

    SMactivity_transIntern_Expect('C');

    /* Re-init. FSM (directly to testing state to reduce redundancy in tests;
       UML SM diagram deviation where init. trans. activity is exec. first) */
    SMfsm_init(SMstatHndlr_p_fsm, p_statHndlrExp);

    evtHandled = SMfsm_sendEvt(SMstatHndlr_p_fsm, SMSTATHNDLR_EVT_C);

    TEST_ASSERT_TRUE(evtHandled);

    p_statHndlrAct = SMfsm_getStat(SMstatHndlr_p_fsm);

    TEST_ASSERT_EQUAL_PTR(p_statHndlrExp, p_statHndlrAct);
}

#endif /* TEST */
