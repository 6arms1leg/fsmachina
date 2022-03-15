/** \file */

#ifndef SMFSM_H
#define SMFSM_H

/* `"` used intentionally.  This allows the user to override and provide his
 * own implementation before falling back to libc.
 */
#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"
#include "assert.h" /* For sanity checks (Design by Contract) */

/* ATTRIBUTES
 * ==========
 */

enum en_fsm_evt
{
    en_fsm_evt_ENTRY = 0,
    en_fsm_evt_EXIT,
    en_fsm_evt_APPEVTSTART
};

/* Forward declaration because of cicrular dependencies */
typedef struct stc_fsm stc_fsm_t;

/* Important: The state handler functions with this signature must be reentrant
 */
typedef bool (*p_fn_sHndlr_t)(stc_fsm_t* const me, const uint8_t u8_evt);

struct stc_fsm
{
    p_fn_sHndlr_t p_fn_state;
};

/* OPERATIONS
 * ==========
 */

void SMfsm_init(stc_fsm_t* const me, const p_fn_sHndlr_t p_fn_sHndlr);

bool SMfsm_sendEvt(stc_fsm_t* const me, const uint8_t u8_evt);

p_fn_sHndlr_t SMfsm_getStat(const stc_fsm_t* const me);

void SMfsm_trans(stc_fsm_t* const me, const p_fn_sHndlr_t p_fn_sHndlr);

#endif /* SMFSM_H */
