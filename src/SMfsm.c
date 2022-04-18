/** \file */

#include "SMfsm.h"

/* OPERATIONS
 * ==========
 */

void SMfsm_init(SMfsm_fsm_t* const me, const SMfsm_p_statHndlr_t p_statHndlr) {
    /* Sanity check (Design by Contract) */
    assert((NULL != me) &&
           (NULL != p_statHndlr));

    me->p_statHndlr = p_statHndlr; /* Change to init. state without any exec. */
}

bool SMfsm_sendEvt(SMfsm_fsm_t* const me, const uint8_t evt) {
    /* Sanity check (Design by Contract) */
    assert((NULL != me) &&
           (NULL != me->p_statHndlr));

    return ((*me->p_statHndlr)(me, evt)); /* Let current state handle event */
}

SMfsm_p_statHndlr_t SMfsm_getStat(const SMfsm_fsm_t* const me) {
    assert(NULL != me); /* Sanity check (Design by Contract) */

    return (me->p_statHndlr);
}

void SMfsm_trans(SMfsm_fsm_t* const me, const SMfsm_p_statHndlr_t p_statHndlr) {
    /* Sanity check (Design by Contract) */
    assert((NULL != me) &&
           (NULL != me->p_statHndlr) &&
           (NULL != p_statHndlr));

    /* Trans. activity can only be exec. from state handler *before* trans. */

    (void)SMfsm_sendEvt(me, SMFSM_EXIT); /* Exec. exit activity of old state */
    me->p_statHndlr = p_statHndlr; /* Change state */
    (void)SMfsm_sendEvt(me, SMFSM_ENTRY); /* Exec. entry activity of new state */
}
