/** \file */

#include "SMhndlr.h"

/* ATTRIBUTES
 * ==========
 */

static bool pv_grd0 = false; /* Extended state variable */

/* OPERATIONS
 * ==========
 */

void SMhndlr_setGrd0(const bool grd) {
    pv_grd0 = grd;
}

/* State handler functions
 */

bool SMhndlr_statInit(SMfsm_fsm_t* const me, const uint8_t evt) {
    bool evtHandled = false;

    switch (evt) {
        case SMFSM_ENTRY: {
            SMactivity_entry();
            SMactivity_transInit();
            SMfsm_trans(me, SMhndlr_statA);
            evtHandled = true;
            break;
        }
        case SMFSM_EXIT: {
            SMactivity_exit();
            evtHandled = true;
            break;
        }
        default: {
            SMactivity_ignore(); /* Ignore unknown events */
        }
    }

    return (evtHandled);
}

bool SMhndlr_statA(SMfsm_fsm_t* const me, const uint8_t evt) {
    bool evtHandled = false;

    switch (evt) {
        case SMFSM_ENTRY: {
            SMactivity_entry();
            evtHandled = true;
            break;
        }
        case SMFSM_EXIT: {
            SMactivity_exit();
            evtHandled = true;
            break;
        }
        case SMHNDLR_EVT_B: {
            SMactivity_trans();
            SMfsm_trans(me, SMhndlr_statB);
            evtHandled = true;
            break;
        }
        default: {
            SMactivity_ignore(); /* Ignore unknown events */
        }
    }

    return (evtHandled);
}

bool SMhndlr_statB(SMfsm_fsm_t* const me, const uint8_t evt) {
    bool evtHandled = false;

    switch (evt) {
        case SMFSM_ENTRY: {
            SMactivity_entry();
            evtHandled = true;
            break;
        }
        case SMFSM_EXIT: {
            SMactivity_exit();
            evtHandled = true;
            break;
        }
        case SMHNDLR_EVT_B: {
            SMactivity_trans();
            SMfsm_trans(me, SMhndlr_statB);
            evtHandled = true;
            break;
        }
        case SMHNDLR_EVT_C: {
            SMactivity_trans();
            SMfsm_trans(me, SMhndlr_statC);
            evtHandled = true;
            break;
        }
        default: {
            SMactivity_ignore(); /* Ignore unknown events */
        }
    }

    return (evtHandled);
}

bool SMhndlr_statC(SMfsm_fsm_t* const me, const uint8_t evt) {
    bool evtHandled = false;

    switch (evt) {
        case SMFSM_ENTRY: {
            SMactivity_entry();
            evtHandled = true;
            break;
        }
        case SMFSM_EXIT: {
            SMactivity_exit();
            evtHandled = true;
            break;
        }
        case SMHNDLR_EVT_A: {
            SMactivity_trans();
            SMfsm_trans(me, SMhndlr_statA);
            evtHandled = true;
            break;
        }
        case SMHNDLR_EVT_B: {
            if (false == pv_grd0) {
                SMactivity_trans();
                SMfsm_trans(me, SMhndlr_statC);
                evtHandled = true;
            }
            else if (true == pv_grd0) {
                SMactivity_trans();
                SMfsm_trans(me, SMhndlr_statB);
                evtHandled = true;
            }
            else {;} /* Do nothing */
            break;
        }
        case SMHNDLR_EVT_C: {
            SMactivity_trans();
            evtHandled = true;
            break;
        }
        default: {
            SMactivity_ignore(); /* Ignore unknown events */
        }
    }

    return (evtHandled);
}

bool SMhndlr_statZ(SMfsm_fsm_t* const me, const uint8_t evt) {
    bool evtHandled = false;

    switch (evt) {
        case SMFSM_ENTRY: {
            SMactivity_entry();
            evtHandled = true;
            break;
        }
        case SMFSM_EXIT: {
            SMactivity_exit();
            evtHandled = true;
            break;
        }
        case SMHNDLR_EVT_Z: {
            /* Explicit type downcast needed (safe as long as `me` input
               argument always points to derived class instance).
               In other words, this state handler function must only be used
               with `me` pointers of type `SMhndlr_fsm_t*` (derived class) and
               never `SMfsm_fsm_t` (base class), as the function’s signature
               might suggests. */
            if (true == ((SMhndlr_fsm_t*)me)->grd1) {
                SMactivity_trans();
                SMfsm_trans(me, SMhndlr_statZ);
                evtHandled = true;
            }
            break;
        }
        default: {
            SMactivity_ignore(); /* Ignore unknown events */
        }
    }

    return (evtHandled);
}
