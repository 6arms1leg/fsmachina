/** \file */

#ifndef SMSTATHNDLR_H
#define SMSTATHNDLR_H

#include <stdint.h>
#include <stdbool.h>

#include "SMfsm.h"

/** \brief User-defined state machine events */
enum SMstatHndlr_evt {
    SMSTATHNDLR_EVT_A = SMFSM_APP_EVT_START,
    SMSTATHNDLR_EVT_B,
    SMSTATHNDLR_EVT_C,
    SMSTATHNDLR_EVT_Z
};

/* ATTRIBUTES
 * ==========
 */

/**
 * \{
 * \brief Global opaque pointers to FSM objects
 */
extern SMfsm_fsm_t* const SMstatHndlr_p_fsm;
extern SMfsm_fsm_t* const SMstatHndlr_p_fsmZ;
/** \} */

/* OPERATIONS
 * ==========
 */

/** \brief Set extended state variable */
void SMstatHndlr_setGrd0(const bool grd);

/**
 * \{
 * \brief State handler functions
 *
 * Implementation of state handler functions defines state machine
 * configuration (i.e. state topology).
 */
bool SMstatHndlr_statA(SMfsm_fsm_t* const me, const uint8_t evt);
bool SMstatHndlr_statB(SMfsm_fsm_t* const me, const uint8_t evt);
bool SMstatHndlr_statC(SMfsm_fsm_t* const me, const uint8_t evt);
/** \} */

/**
 * \brief State handler function that uses extended state variable `grd1`;
 * derived from `SMfsm_fsm_t` base class
 */
bool SMstatHndlr_statZ(SMfsm_fsm_t* const me, const uint8_t evt);

/**
 * \{
 * \brief Constructors of FSM objects
 */
void SMstatHndlr_fsmCtor(void);
void SMstatHndlr_fsmZCtor(void);
/** \} */

#endif /* SMSTATHNDLR_H */
