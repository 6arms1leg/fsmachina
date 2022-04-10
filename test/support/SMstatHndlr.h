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

/* ATTRIBUTES
 * ==========
 */

/* Global opaque pointers to FSM objects */
extern SMfsm_fsm_t* const SMstatHndlr_p_fsm;
extern SMfsm_fsm_t* const SMstatHndlr_p_fsmZ;

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

/* Constructors of FSM objects */
void SMstatHndlr_fsmCtor(void);
void SMstatHndlr_fsmZCtor(void);

#endif /* SMSTATHNDLR_H */
