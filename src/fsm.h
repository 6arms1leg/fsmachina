/** \file */

#ifndef FSM_H
#define FSM_H

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

void fn_fsm_ini(stc_fsm_t* const me, const p_fn_sHndlr_t p_fn_sHndlr);

bool fn_fsm_sendEvt(stc_fsm_t* const me, const uint8_t u8_evt);

p_fn_sHndlr_t fn_fsm_getState(const stc_fsm_t* const me);

void fn_fsm_tran(stc_fsm_t* const me, const p_fn_sHndlr_t p_fn_sHndlr);

#endif /* FSM_H */
