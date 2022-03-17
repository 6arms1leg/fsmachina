/** \file */

#ifndef SMHNDLR_H
#define SMHNDLR_H

#include "SMfsm.h"
#include "SMactivity.h"

/* `"` used intentionally.  This allows the user to override and provide his
 * own implementation before falling back to libc.
 */
#include "stdint.h"
#include "stdbool.h"

/* ATTRIBUTES
 * ==========
 */

enum SMhndlr_evt
{
    SMHNDLR_EVT_A = SMFSM_APP_EVT_START,
    SMHNDLR_EVT_B,
    SMHNDLR_EVT_C,
    SMHNDLR_EVT_Z
};

/* New FSM structure with extended state variable `grd1`; derived from
 * `SMfsm_fsm_t` base class
 */
typedef struct
{
    SMfsm_fsm_t super;
    bool b_grd1;
} SMhndlr_fsm_t;

/* OPERATIONS
 * ==========
 */

/* Extended state variable `grd0` access operation */
void SMhndlr_setGrd0(const bool grd);

/* State handler functions
 *
 * The implementation of the state handler functions defines the state machine
 * configuration (i.e. the state topology).
 */
bool SMhndlr_statInit(SMfsm_fsm_t* const me, const uint8_t u8_evt);
bool SMhndlr_statA(SMfsm_fsm_t* const me, const uint8_t u8_evt);
bool SMhndlr_statB(SMfsm_fsm_t* const me, const uint8_t u8_evt);
bool SMhndlr_statC(SMfsm_fsm_t* const me, const uint8_t u8_evt);

/* State handler function that uses the extended state variable `grd1`; derived
 * from `SMfsm_fsm_t` base class */
bool SMhndlr_statZ(SMfsm_fsm_t* const me, const uint8_t u8_evt);

#endif /* SMHNDLR_H */
