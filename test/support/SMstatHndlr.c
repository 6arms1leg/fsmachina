/** \file */

#include "SMstatHndlr.h"

#include "SMactivity.h"

/**
 * \brief New FSM structure with extended state variable; derived from base
 * class
 */
typedef struct {
    SMfsm_fsm_t base; /**< \brief Base class */
    bool grd1; /**< \brief Extended state variable */
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
    /* No sanity checks necessary; `me` arg. already checked in API functions,
       unknown event arg. ignored anyway */

    bool evtHandled = false;

    switch (evt) {
        case SMFSM_ENTRY: {
            SMactivity_entry('0');
            SMactivity_trans('0', 'A');
            SMfsm_trans(me, &SMstatHndlr_statA);
            evtHandled = true;
            break;
        }
        case SMFSM_EXIT: {
            SMactivity_exit('0');
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
    /* No sanity checks necessary; `me` arg. already checked in API functions,
       unknown event arg. ignored anyway */

    bool evtHandled = false;

    switch (evt) {
        case SMFSM_ENTRY: {
            SMactivity_entry('A');
            evtHandled = true;
            break;
        }
        case SMFSM_EXIT: {
            SMactivity_exit('A');
            evtHandled = true;
            break;
        }
        case SMSTATHNDLR_EVT_B: {
            SMactivity_trans('A', 'B');
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
    /* No sanity checks necessary; `me` arg. already checked in API functions,
       unknown event arg. ignored anyway */

    bool evtHandled = false;

    switch (evt) {
        case SMFSM_ENTRY: {
            SMactivity_entry('B');
            evtHandled = true;
            break;
        }
        case SMFSM_EXIT: {
            SMactivity_exit('B');
            evtHandled = true;
            break;
        }
        case SMSTATHNDLR_EVT_B: {
            SMactivity_trans('B', 'B');
            SMfsm_trans(me, &SMstatHndlr_statB);
            evtHandled = true;
            break;
        }
        case SMSTATHNDLR_EVT_C: {
            SMactivity_trans('B', 'C');
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
    /* No sanity checks necessary; `me` arg. already checked in API functions,
       unknown event arg. ignored anyway */

    bool evtHandled = false;

    switch (evt) {
        case SMFSM_ENTRY: {
            SMactivity_entry('C');
            evtHandled = true;
            break;
        }
        case SMFSM_EXIT: {
            SMactivity_exit('C');
            evtHandled = true;
            break;
        }
        case SMSTATHNDLR_EVT_A: {
            SMactivity_trans('C', 'A');
            SMfsm_trans(me, &SMstatHndlr_statA);
            evtHandled = true;
            break;
        }
        case SMSTATHNDLR_EVT_B: {
            /* Trans. guard */
            if (false == pv_grd0) { /* Guard condition false? */
                SMactivity_trans('C', 'C');
                SMfsm_trans(me, &SMstatHndlr_statC); /* Trans. to self */
                evtHandled = true;
            }
            else if (true == pv_grd0) { /* Guard condition true? */
                SMactivity_trans('C', 'B');
                SMfsm_trans(me, &SMstatHndlr_statB);
                evtHandled = true;
            }
            else {;} /* Do nothing (MISRA-required) */
            break;
        }
        case SMSTATHNDLR_EVT_C: {
            SMactivity_transIntern('C');
            evtHandled = true;
            break;
        }
        default: {
            SMactivity_ignore(); /* Ignore unknown events */
        }
    } /* switch (...) */

    return (evtHandled);
}

/* Only used to demonstrate utilization of extended state variables via
   inheritance */
bool SMstatHndlr_statZ(SMfsm_fsm_t* const me, const uint8_t evt) {
    /* No sanity checks necessary; `me` arg. already checked in API functions,
       unknown event arg. ignored anyway */

    bool evtHandled = false;

    switch (evt) {
        case SMFSM_ENTRY: {
            SMactivity_entry('Z');
            evtHandled = true;
            break;
        }
        case SMFSM_EXIT: {
            SMactivity_exit('Z');
            evtHandled = true;
            break;
        }
        case SMSTATHNDLR_EVT_Z: {
            /* Trans. guard:
               Explicit type downcast needed (safe as long as `me` input
               argument always points to derived class instance).
               In other words, this state handler function must only be used
               with `me` pointers of type `SMstatHndlr_fsm_t*` (derived class)
               and never `SMfsm_fsm_t` (base class), as the function’s
               signature might suggest. */
            if (true == ((SMstatHndlr_fsm_t*)me)->grd1) { /* Guard cond. true? */
                SMactivity_trans('Z', 'Z');
                SMfsm_trans(me, &SMstatHndlr_statZ); /* Trans. to self */
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

/* FSM object contructor functions
 */

void SMstatHndlr_fsmCtor(void) {
    SMfsm_init(&pv_fsm, &statInit);
}

void SMstatHndlr_fsmZCtor(void) {
    SMfsm_init((SMfsm_fsm_t*)&pv_fsmZ, &SMstatHndlr_statZ);
        /* Explicit type upcast needed (safe here) */
    pv_fsmZ.grd1 = true; /* Set extended state variable */
}
