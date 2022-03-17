/** \file */

#include "SMfsm.h"

/* OPERATIONS
 * ==========
 */

void SMfsm_init(SMfsm_fsm_t* const me, const SMfsm_p_hndlr_t p_fn_sHndlr)
{
    /* Sanity check (Design by Contract) */
    assert( (NULL != me) &&
            (NULL != p_fn_sHndlr) );

    me->p_fn_state = p_fn_sHndlr;

    return;
}

bool SMfsm_sendEvt(SMfsm_fsm_t* const me, const uint8_t u8_evt)
{
    /* Sanity check (Design by Contract) */
    assert(NULL != me);

    return( (*me->p_fn_state)(me, u8_evt) );
}

SMfsm_p_hndlr_t SMfsm_getStat(const SMfsm_fsm_t* const me)
{
    /* Sanity check (Design by Contract) */
    assert(NULL != me);

    return(me->p_fn_state);
}

void SMfsm_trans(SMfsm_fsm_t* const me, const SMfsm_p_hndlr_t p_fn_sHndlr)
{
    /* Sanity check (Design by Contract) */
    assert( (NULL != me) &&
            (NULL != p_fn_sHndlr) );

    (void)SMfsm_sendEvt(me, SMFSM_EXIT);
    me->p_fn_state = p_fn_sHndlr;
    (void)SMfsm_sendEvt(me, SMFSM_ENTRY);

    return;
}
