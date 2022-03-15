/** \file */

#include "SMhndlr.h"

/* ATTRIBUTES
 * ==========
 */

/* Extended state variables */
static bool b_pv_grd0 = false;

/* OPERATIONS
 * ==========
 */

void SMhndlr_setGrd0(const bool b_grd)
{
    b_pv_grd0 = b_grd;

    return;
}

/* State handler functions */
bool SMhndlr_statInit(stc_fsm_t* const me, const uint8_t u8_evt)
{
    bool b_evtHandled = false;

    switch(u8_evt)
    {
        case en_fsm_evt_ENTRY:
            SMactivity_entry();
            SMactivity_transInit();
            SMfsm_trans(me, SMhndlr_statA);
            b_evtHandled = true;
            break;
        case en_fsm_evt_EXIT:
            SMactivity_exit();
            b_evtHandled = true;
            break;
        default:
            /* Ignore unknown events */
            SMactivity_ignore();
    }

    return(b_evtHandled);
}

bool SMhndlr_statA(stc_fsm_t* const me, const uint8_t u8_evt)
{
    bool b_evtHandled = false;

    switch(u8_evt)
    {
        case en_fsm_evt_ENTRY:
            SMactivity_entry();
            b_evtHandled = true;
            break;
        case en_fsm_evt_EXIT:
            SMactivity_exit();
            b_evtHandled = true;
            break;
        case en_sHndlr_evt_B:
            SMactivity_trans();
            SMfsm_trans(me, SMhndlr_statB);
            b_evtHandled = true;
            break;
        default:
            /* Ignore unknown events */
            SMactivity_ignore();
    }

    return(b_evtHandled);
}

bool SMhndlr_statB(stc_fsm_t* const me, const uint8_t u8_evt)
{
    bool b_evtHandled = false;

    switch(u8_evt)
    {
        case en_fsm_evt_ENTRY:
            SMactivity_entry();
            b_evtHandled = true;
            break;
        case en_fsm_evt_EXIT:
            SMactivity_exit();
            b_evtHandled = true;
            break;
        case en_sHndlr_evt_B:
            SMactivity_trans();
            SMfsm_trans(me, SMhndlr_statB);
            b_evtHandled = true;
            break;
        case en_sHndlr_evt_C:
            SMactivity_trans();
            SMfsm_trans(me, SMhndlr_statC);
            b_evtHandled = true;
            break;
        default:
            /* Ignore unknown events */
            SMactivity_ignore();
    }

    return(b_evtHandled);
}

bool SMhndlr_statC(stc_fsm_t* const me, const uint8_t u8_evt)
{
    bool b_evtHandled = false;

    switch(u8_evt)
    {
        case en_fsm_evt_ENTRY:
            SMactivity_entry();
            b_evtHandled = true;
            break;
        case en_fsm_evt_EXIT:
            SMactivity_exit();
            b_evtHandled = true;
            break;
        case en_sHndlr_evt_A:
            SMactivity_trans();
            SMfsm_trans(me, SMhndlr_statA);
            b_evtHandled = true;
            break;
        case en_sHndlr_evt_B:
            if(false == b_pv_grd0)
            {
                SMactivity_trans();
                SMfsm_trans(me, SMhndlr_statC);
                b_evtHandled = true;
            }
            else if(true == b_pv_grd0)
            {
                SMactivity_trans();
                SMfsm_trans(me, SMhndlr_statB);
                b_evtHandled = true;
            }
            else { ; /* Do nothing */ }
            break;
        case en_sHndlr_evt_C:
            SMactivity_trans();
            b_evtHandled = true;
            break;
        default:
            /* Ignore unknown events */
            SMactivity_ignore();
    }

    return(b_evtHandled);
}

bool SMhndlr_statZ(stc_fsm_t* const me, const uint8_t u8_evt)
{
    bool b_evtHandled = false;

    switch(u8_evt)
    {
        case en_fsm_evt_ENTRY:
            SMactivity_entry();
            b_evtHandled = true;
            break;
        case en_fsm_evt_EXIT:
            SMactivity_exit();
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
                SMactivity_trans();
                SMfsm_trans(me, SMhndlr_statZ);
                b_evtHandled = true;
            }
            break;
        default:
            /* Ignore unknown events */
            SMactivity_ignore();
    }

    return(b_evtHandled);
}
