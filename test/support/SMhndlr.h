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

enum en_sHndlr_evt
{
    en_sHndlr_evt_A = en_fsm_evt_APPEVTSTART,
    en_sHndlr_evt_B,
    en_sHndlr_evt_C,
    en_sHndlr_evt_Z
};

/* New FSM structure with extended state variable `grd1`; derived from
 * `stc_fsm_t` base class
 */
typedef struct
{
    stc_fsm_t super;
    bool b_grd1;
} stc_sHndlr_fsm_t;

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
bool SMhndlr_statInit(stc_fsm_t* const me, const uint8_t u8_evt);
bool SMhndlr_statA(stc_fsm_t* const me, const uint8_t u8_evt);
bool SMhndlr_statB(stc_fsm_t* const me, const uint8_t u8_evt);
bool SMhndlr_statC(stc_fsm_t* const me, const uint8_t u8_evt);

/* State handler function that uses the extended state variable `grd1`; derived
 * from `stc_fsm_t` base class */
bool SMhndlr_statZ(stc_fsm_t* const me, const uint8_t u8_evt);

#endif /* SMHNDLR_H */
