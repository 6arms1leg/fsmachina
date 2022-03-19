/** \file */

#include "SMfsm.h"

/* OPERATIONS
 * ==========
 */

void SMfsm_init(SMfsm_fsm_t* const me, const SMfsm_p_hndlr_t p_statHndlr)
{
    /* Sanity check (Design by Contract) */
    assert( (NULL != me) &&
            (NULL != p_statHndlr) );

    me->p_statHndlr = p_statHndlr;

    return;
}

bool SMfsm_sendEvt(SMfsm_fsm_t* const me, const uint8_t evt)
{
    /* Sanity check (Design by Contract) */
    assert(NULL != me);

    return( (*me->p_statHndlr)(me, evt) );
}

SMfsm_p_hndlr_t SMfsm_getStat(const SMfsm_fsm_t* const me)
{
    /* Sanity check (Design by Contract) */
    assert(NULL != me);

    return(me->p_statHndlr);
}

void SMfsm_trans(SMfsm_fsm_t* const me, const SMfsm_p_hndlr_t p_statHndlr)
{
    /* Sanity check (Design by Contract) */
    assert( (NULL != me) &&
            (NULL != p_statHndlr) );

    (void)SMfsm_sendEvt(me, SMFSM_EXIT);
    me->p_statHndlr = p_statHndlr;
    (void)SMfsm_sendEvt(me, SMFSM_ENTRY);

    return;
}
