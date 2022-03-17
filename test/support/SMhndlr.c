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
bool SMhndlr_statInit(SMfsm_fsm_t* const me, const uint8_t u8_evt)
{
    bool b_evtHandled = false;

    switch(u8_evt)
    {
        case SMFSM_ENTRY:
            SMactivity_entry();
            SMactivity_transInit();
            SMfsm_trans(me, SMhndlr_statA);
            b_evtHandled = true;
            break;
        case SMFSM_EXIT:
            SMactivity_exit();
            b_evtHandled = true;
            break;
        default:
            /* Ignore unknown events */
            SMactivity_ignore();
    }

    return(b_evtHandled);
}

bool SMhndlr_statA(SMfsm_fsm_t* const me, const uint8_t u8_evt)
{
    bool b_evtHandled = false;

    switch(u8_evt)
    {
        case SMFSM_ENTRY:
            SMactivity_entry();
            b_evtHandled = true;
            break;
        case SMFSM_EXIT:
            SMactivity_exit();
            b_evtHandled = true;
            break;
        case SMHNDLR_EVT_B:
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

bool SMhndlr_statB(SMfsm_fsm_t* const me, const uint8_t u8_evt)
{
    bool b_evtHandled = false;

    switch(u8_evt)
    {
        case SMFSM_ENTRY:
            SMactivity_entry();
            b_evtHandled = true;
            break;
        case SMFSM_EXIT:
            SMactivity_exit();
            b_evtHandled = true;
            break;
        case SMHNDLR_EVT_B:
            SMactivity_trans();
            SMfsm_trans(me, SMhndlr_statB);
            b_evtHandled = true;
            break;
        case SMHNDLR_EVT_C:
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

bool SMhndlr_statC(SMfsm_fsm_t* const me, const uint8_t u8_evt)
{
    bool b_evtHandled = false;

    switch(u8_evt)
    {
        case SMFSM_ENTRY:
            SMactivity_entry();
            b_evtHandled = true;
            break;
        case SMFSM_EXIT:
            SMactivity_exit();
            b_evtHandled = true;
            break;
        case SMHNDLR_EVT_A:
            SMactivity_trans();
            SMfsm_trans(me, SMhndlr_statA);
            b_evtHandled = true;
            break;
        case SMHNDLR_EVT_B:
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
        case SMHNDLR_EVT_C:
            SMactivity_trans();
            b_evtHandled = true;
            break;
        default:
            /* Ignore unknown events */
            SMactivity_ignore();
    }

    return(b_evtHandled);
}

bool SMhndlr_statZ(SMfsm_fsm_t* const me, const uint8_t u8_evt)
{
    bool b_evtHandled = false;

    switch(u8_evt)
    {
        case SMFSM_ENTRY:
            SMactivity_entry();
            b_evtHandled = true;
            break;
        case SMFSM_EXIT:
            SMactivity_exit();
            b_evtHandled = true;
            break;
        case SMHNDLR_EVT_Z:
            /* Explicit downcast needed (which is safe as long as the `me`
             * input argument always points to a derived class instance).
             * In other words, this state handler function must only be used
             * with `me` pointers of type `SMhndlr_fsm_t*` (derived class) and
             * never `SMfsm_fsm_t` (base class), as the function’s signature
             * might suggests.
             */
            if(true == ( (SMhndlr_fsm_t*)me )->b_grd1)
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
