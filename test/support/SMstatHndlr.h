/** \file */

#ifndef SMSTATHNDLR_H
#define SMSTATHNDLR_H

#include <stdint.h>
#include <stdbool.h>

#include "SMfsm.h"

enum SMstatHndlr_evt {
    SMSTATHNDLR_EVT_A = SMFSM_APP_EVT_START,
    SMSTATHNDLR_EVT_B,
    SMSTATHNDLR_EVT_C,
    SMSTATHNDLR_EVT_Z
};

/* New FSM structure with extended state variable; derived from base class */
typedef struct {
    SMfsm_fsm_t super; /* Base class */
    bool grd1; /* Extended state variable */
} SMstatHndlr_fsm_t;

/* ATTRIBUTES
 * ==========
 */

/* Global opaque pointer to FSM object */
extern SMfsm_fsm_t* const SMstatHndlr_p_fsm;

/* OPERATIONS
 * ==========
 */

/* Extended state variable access operation */
void SMstatHndlr_setGrd0(const bool grd);

/* State handler functions
 *
 * The implementation of the state handler functions defines the state machine
 * configuration (i.e. the state topology).
 */
bool SMstatHndlr_statA(SMfsm_fsm_t* const me, const uint8_t evt);
bool SMstatHndlr_statB(SMfsm_fsm_t* const me, const uint8_t evt);
bool SMstatHndlr_statC(SMfsm_fsm_t* const me, const uint8_t evt);

/* State handler function that uses extended state variable `grd1`; derived
 * from `SMfsm_fsm_t` base class
 */
bool SMstatHndlr_statZ(SMfsm_fsm_t* const me, const uint8_t evt);

/* Constructor of FSM object */
void SMstatHndlr_fsmCtor(void);

#endif /* SMSTATHNDLR_H */
