/** \file */

#include "SMfsm.h"

/* OPERATIONS
 * ==========
 */

void fn_fsm_ini(stc_fsm_t* const me, const p_fn_sHndlr_t p_fn_sHndlr)
{
    /* Sanity check (Design by Contract) */
    assert( (NULL != me) &&
            (NULL != p_fn_sHndlr) );

    me->p_fn_state = p_fn_sHndlr;

    return;
}

bool fn_fsm_sendEvt(stc_fsm_t* const me, const uint8_t u8_evt)
{
    /* Sanity check (Design by Contract) */
    assert(NULL != me);

    return( (*me->p_fn_state)(me, u8_evt) );
}

p_fn_sHndlr_t fn_fsm_getState(const stc_fsm_t* const me)
{
    /* Sanity check (Design by Contract) */
    assert(NULL != me);

    return(me->p_fn_state);
}

void fn_fsm_tran(stc_fsm_t* const me, const p_fn_sHndlr_t p_fn_sHndlr)
{
    /* Sanity check (Design by Contract) */
    assert( (NULL != me) &&
            (NULL != p_fn_sHndlr) );

    (void)fn_fsm_sendEvt(me, en_fsm_evt_EXIT);
    me->p_fn_state = p_fn_sHndlr;
    (void)fn_fsm_sendEvt(me, en_fsm_evt_ENTRY);

    return;
}
