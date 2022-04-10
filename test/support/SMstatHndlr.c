/** \file */

#include "SMstatHndlr.h"

#include "SMactivity.h"

/* New FSM structure with extended state variable; derived from base class */
typedef struct {
    SMfsm_fsm_t base; /* Base class */
    bool grd1; /* Extended state variable */
} SMstatHndlr_fsm_t;

/* ATTRIBUTES
 * ==========
 */

/* Private FSM instances */
static SMfsm_fsm_t pv_fsm;
static SMstatHndlr_fsm_t pv_fsmZ;

/* Global opaque pointers to FSM objects */
SMfsm_fsm_t* const SMstatHndlr_p_fsm = &pv_fsm;
SMfsm_fsm_t* const SMstatHndlr_p_fsmZ = (SMfsm_fsm_t*)&pv_fsmZ;
    /* Explicit type upcast needed (safe here) */

/* Extended state variable */
static bool pv_grd0;

/* OPERATIONS
 * ==========
 */

void SMstatHndlr_setGrd0(const bool grd) {
    pv_grd0 = grd;
}

/* State handler functions
 */

static bool statInit(SMfsm_fsm_t* const me, const uint8_t evt) {
    bool evtHandled = false;

    switch (evt) {
        case SMFSM_ENTRY: {
            SMactivity_entry();
            SMactivity_transInit();
            SMfsm_trans(me, &SMstatHndlr_statA);
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

bool SMstatHndlr_statA(SMfsm_fsm_t* const me, const uint8_t evt) {
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
        case SMSTATHNDLR_EVT_B: {
            SMactivity_trans();
            SMfsm_trans(me, &SMstatHndlr_statB);
            evtHandled = true;
            break;
        }
        default: {
            SMactivity_ignore(); /* Ignore unknown events */
        }
    }

    return (evtHandled);
}

bool SMstatHndlr_statB(SMfsm_fsm_t* const me, const uint8_t evt) {
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
        case SMSTATHNDLR_EVT_B: {
            SMactivity_trans();
            SMfsm_trans(me, &SMstatHndlr_statB);
            evtHandled = true;
            break;
        }
        case SMSTATHNDLR_EVT_C: {
            SMactivity_trans();
            SMfsm_trans(me, &SMstatHndlr_statC);
            evtHandled = true;
            break;
        }
        default: {
            SMactivity_ignore(); /* Ignore unknown events */
        }
    } /* switch (...) */

    return (evtHandled);
}

bool SMstatHndlr_statC(SMfsm_fsm_t* const me, const uint8_t evt) {
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
        case SMSTATHNDLR_EVT_A: {
            SMactivity_trans();
            SMfsm_trans(me, &SMstatHndlr_statA);
            evtHandled = true;
            break;
        }
        case SMSTATHNDLR_EVT_B: {
            if (false == pv_grd0) {
                SMactivity_trans();
                SMfsm_trans(me, &SMstatHndlr_statC);
                evtHandled = true;
            }
            else if (true == pv_grd0) {
                SMactivity_trans();
                SMfsm_trans(me, &SMstatHndlr_statB);
                evtHandled = true;
            }
            else {;} /* Do nothing */
            break;
        }
        case SMSTATHNDLR_EVT_C: {
            SMactivity_trans();
            evtHandled = true;
            break;
        }
        default: {
            SMactivity_ignore(); /* Ignore unknown events */
        }
    } /* switch (...) */

    return (evtHandled);
}

bool SMstatHndlr_statZ(SMfsm_fsm_t* const me, const uint8_t evt) {
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
        case SMSTATHNDLR_EVT_Z: {
            /* Explicit type downcast needed (safe as long as `me` input
               argument always points to derived class instance).
               In other words, this state handler function must only be used
               with `me` pointers of type `SMstatHndlr_fsm_t*` (derived class)
               and never `SMfsm_fsm_t` (base class), as the function’s
               signature might suggests. */
            if (true == ((SMstatHndlr_fsm_t*)me)->grd1) {
                SMactivity_trans();
                SMfsm_trans(me, &SMstatHndlr_statZ);
                evtHandled = true;
            }
            break;
        }
        default: {
            SMactivity_ignore(); /* Ignore unknown events */
        }
    } /* switch (...) */

    return (evtHandled);
}

void SMstatHndlr_fsmCtor(void) {
    SMfsm_init(&pv_fsm, &statInit);
}

void SMstatHndlr_fsmZCtor(void) {
    SMfsm_init((SMfsm_fsm_t*)&pv_fsmZ, &SMstatHndlr_statZ);
        /* Explicit type upcast needed (safe here) */
    pv_fsmZ.grd1 = true; /* Set extended state variable */
}
