/** \file */

#include "main.h"

#include "strMsg.h"

#include "SMstatHndlr.h"

/* OPERATIONS
 * ==========
 */

/**
 * \brief Convert state handler function pointer to printable format
 *
 * \param p_stat state handler
 *
 * \return Printable state name
 */
static char convStat(SMfsm_p_statHndlr_t p_stat) {
    char statName = '0'; /* Default (FSM not init./started) */

    if (&SMstatHndlr_statA == p_stat) {
        statName = 'A';
    }
    else if (&SMstatHndlr_statB == p_stat) {
        statName = 'B';
    }
    else if (&SMstatHndlr_statC == p_stat) {
        statName = 'C';
    }
    else if (&SMstatHndlr_statZ == p_stat) {
        statName = 'Z';
    }
    else {;} /* Do nothing (MISRA-required) */

    return (statName);
}

int MAIN(void) {
    (void)puts(APP_PROMPT
               "~~~ Welcome to the FSMachina FSM example application! ~~~");

    /* FSM; states A, B, C
     */

    (void)puts(APP_PROMPT CTOR_MSG());
    SMstatHndlr_fsmCtor(); /* Init. FSM object */

    (void)printf(APP_PROMPT "Current state:  %c\n",
                 convStat(SMfsm_getStat(SMstatHndlr_p_fsm)));

    (void)puts(APP_PROMPT START_MSG());
    (void)SMfsm_sendEvt(SMstatHndlr_p_fsm, SMFSM_ENTRY); /* Start FSM */

    (void)printf(APP_PROMPT "Current state:  %c\n",
                 convStat(SMfsm_getStat(SMstatHndlr_p_fsm)));

    (void)puts(APP_PROMPT SEND_MSG(A));
    (void)SMfsm_sendEvt(SMstatHndlr_p_fsm, SMSTATHNDLR_EVT_A); /* Ignored */
    (void)puts(APP_PROMPT SEND_MSG(B));
    (void)SMfsm_sendEvt(SMstatHndlr_p_fsm, SMSTATHNDLR_EVT_B);

    (void)printf(APP_PROMPT "Current state:  %c\n",
                 convStat(SMfsm_getStat(SMstatHndlr_p_fsm)));

    (void)puts(APP_PROMPT SEND_MSG(B));
    (void)SMfsm_sendEvt(SMstatHndlr_p_fsm, SMSTATHNDLR_EVT_B);
    (void)puts(APP_PROMPT SEND_MSG(A));
    (void)SMfsm_sendEvt(SMstatHndlr_p_fsm, SMSTATHNDLR_EVT_A); /* Ignored */
    (void)puts(APP_PROMPT SEND_MSG(C));
    (void)SMfsm_sendEvt(SMstatHndlr_p_fsm, SMSTATHNDLR_EVT_C);

    (void)printf(APP_PROMPT "Current state:  %c\n",
                 convStat(SMfsm_getStat(SMstatHndlr_p_fsm)));

    (void)puts(APP_PROMPT SEND_MSG(Z));
    (void)SMfsm_sendEvt(SMstatHndlr_p_fsm, SMSTATHNDLR_EVT_Z); /* Ignored */
    (void)puts(APP_PROMPT SEND_MSG(B));
    (void)SMfsm_sendEvt(SMstatHndlr_p_fsm, SMSTATHNDLR_EVT_B); /* Guard `false` */
    SMstatHndlr_setGrd0(true);
    (void)puts(APP_PROMPT SEND_MSG(B));
    (void)SMfsm_sendEvt(SMstatHndlr_p_fsm, SMSTATHNDLR_EVT_B); /* Guard `true` */

    (void)printf(APP_PROMPT "Current state:  %c\n",
                 convStat(SMfsm_getStat(SMstatHndlr_p_fsm)));

    (void)puts(APP_PROMPT SEND_MSG(C));
    (void)SMfsm_sendEvt(SMstatHndlr_p_fsm, SMSTATHNDLR_EVT_C);

    (void)printf(APP_PROMPT "Current state:  %c\n",
                 convStat(SMfsm_getStat(SMstatHndlr_p_fsm)));

    (void)puts(APP_PROMPT SEND_MSG(C));
    (void)SMfsm_sendEvt(SMstatHndlr_p_fsm, SMSTATHNDLR_EVT_C);
    (void)puts(APP_PROMPT SEND_MSG(A));
    (void)SMfsm_sendEvt(SMstatHndlr_p_fsm, SMSTATHNDLR_EVT_A);

    (void)printf(APP_PROMPT "Current state:  %c\n",
                 convStat(SMfsm_getStat(SMstatHndlr_p_fsm)));

    /* FSM Z; state Z
     * (simplified FSM; only used to demonstrate util. of ext. state vars. via
     * inheritance)
     */

    (void)puts(APP_PROMPT CTOR_MSG(Z));
    SMstatHndlr_fsmZCtor(); /* Init. FSM object */

    (void)printf(APP_PROMPT "Current state:  %c\n",
                 convStat(SMfsm_getStat(SMstatHndlr_p_fsmZ)));

    /* FSM Z not really started as constructor directly init. it to state Z
     * (for brevity).  But entry activity should still be exec. */
    (void)puts(APP_PROMPT START_MSG(Z));
    (void)SMfsm_sendEvt(SMstatHndlr_p_fsmZ, SMFSM_ENTRY); /* Start FSM */

    (void)printf(APP_PROMPT "Current state:  %c\n",
                 convStat(SMfsm_getStat(SMstatHndlr_p_fsmZ)));

    (void)puts(APP_PROMPT SEND_MSG(A));
    (void)SMfsm_sendEvt(SMstatHndlr_p_fsmZ, SMSTATHNDLR_EVT_A); /* Ignored */
    (void)puts(APP_PROMPT SEND_MSG(Z));
    (void)SMfsm_sendEvt(SMstatHndlr_p_fsmZ, SMSTATHNDLR_EVT_Z);

    (void)printf(APP_PROMPT "Current state:  %c\n",
                 convStat(SMfsm_getStat(SMstatHndlr_p_fsmZ)));

    (void)puts(APP_PROMPT "~~~ Bye! ~~~");

    return (0);
}
