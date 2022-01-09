/** \file */

#include "stateHandler.h"

/* ATTRIBUTES
 * ==========
 */

/* Extended state variables */
static bool b_pv_grd0 = false;

/* OPERATIONS
 * ==========
 */

void fn_sHndlr_setGrd0(const bool b_grd)
{
    b_pv_grd0 = b_grd;

    return;
}

/* State handler functions */
bool fn_sHndlr_stateIni(stc_fsm_t* const me, const uint8_t u8_evt)
{
    bool b_evtHandled = false;

    switch(u8_evt)
    {
        case en_fsm_evt_ENTRY:
            fn_activity_entry();
            fn_activity_tranIni();
            fn_fsm_tran(me, fn_sHndlr_stateA);
            b_evtHandled = true;
            break;
        case en_fsm_evt_EXIT:
            fn_activity_exit();
            b_evtHandled = true;
            break;
        default:
            /* Ignore unknown events */
            fn_activity_ignore();
    }

    return(b_evtHandled);
}

bool fn_sHndlr_stateA(stc_fsm_t* const me, const uint8_t u8_evt)
{
    bool b_evtHandled = false;

    switch(u8_evt)
    {
        case en_fsm_evt_ENTRY:
            fn_activity_entry();
            b_evtHandled = true;
            break;
        case en_fsm_evt_EXIT:
            fn_activity_exit();
            b_evtHandled = true;
            break;
        case en_sHndlr_evt_B:
            fn_activity_tran();
            fn_fsm_tran(me, fn_sHndlr_stateB);
            b_evtHandled = true;
            break;
        default:
            /* Ignore unknown events */
            fn_activity_ignore();
    }

    return(b_evtHandled);
}

bool fn_sHndlr_stateB(stc_fsm_t* const me, const uint8_t u8_evt)
{
    bool b_evtHandled = false;

    switch(u8_evt)
    {
        case en_fsm_evt_ENTRY:
            fn_activity_entry();
            b_evtHandled = true;
            break;
        case en_fsm_evt_EXIT:
            fn_activity_exit();
            b_evtHandled = true;
            break;
        case en_sHndlr_evt_B:
            fn_activity_tran();
            fn_fsm_tran(me, fn_sHndlr_stateB);
            b_evtHandled = true;
            break;
        case en_sHndlr_evt_C:
            fn_activity_tran();
            fn_fsm_tran(me, fn_sHndlr_stateC);
            b_evtHandled = true;
            break;
        default:
            /* Ignore unknown events */
            fn_activity_ignore();
    }

    return(b_evtHandled);
}

bool fn_sHndlr_stateC(stc_fsm_t* const me, const uint8_t u8_evt)
{
    bool b_evtHandled = false;

    switch(u8_evt)
    {
        case en_fsm_evt_ENTRY:
            fn_activity_entry();
            b_evtHandled = true;
            break;
        case en_fsm_evt_EXIT:
            fn_activity_exit();
            b_evtHandled = true;
            break;
        case en_sHndlr_evt_A:
            fn_activity_tran();
            fn_fsm_tran(me, fn_sHndlr_stateA);
            b_evtHandled = true;
            break;
        case en_sHndlr_evt_B:
            if(false == b_pv_grd0)
            {
                fn_activity_tran();
                fn_fsm_tran(me, fn_sHndlr_stateC);
                b_evtHandled = true;
            }
            else if(true == b_pv_grd0)
            {
                fn_activity_tran();
                fn_fsm_tran(me, fn_sHndlr_stateB);
                b_evtHandled = true;
            }
            else { ; /* Do nothing */ }
            break;
        case en_sHndlr_evt_C:
            fn_activity_tran();
            b_evtHandled = true;
            break;
        default:
            /* Ignore unknown events */
            fn_activity_ignore();
    }

    return(b_evtHandled);
}

bool fn_sHndlr_stateZ(stc_fsm_t* const me, const uint8_t u8_evt)
{
    bool b_evtHandled = false;

    switch(u8_evt)
    {
        case en_fsm_evt_ENTRY:
            fn_activity_entry();
            b_evtHandled = true;
            break;
        case en_fsm_evt_EXIT:
            fn_activity_exit();
            b_evtHandled = true;
            break;
        case en_sHndlr_evt_Z:
            /* Explicit downcast needed (which is safe as long as the `me`
             * input argument always points to a derived class instance).
             * In other words, this state handler function must only be used
             * with `me` pointers of type `stc_sHndlr_fsm_t*` (derived class)
             * and never `stc_fsm_t` (base class), as the function’s signature
             * might suggests.
             */
            if(true == ( (stc_sHndlr_fsm_t*)me )->b_grd1)
            {
                fn_activity_tran();
                fn_fsm_tran(me, fn_sHndlr_stateZ);
                b_evtHandled = true;
            }
            break;
        default:
            /* Ignore unknown events */
            fn_activity_ignore();
    }

    return(b_evtHandled);
}
