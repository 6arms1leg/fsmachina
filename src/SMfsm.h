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

enum SMfsm_evt
{
    SMFSM_ENTRY = 0,
    SMFSM_EXIT,
    SMFSM_APP_EVT_START
};

/* Forward declaration because of cicrular dependencies */
typedef struct SMfsm_fsm SMfsm_fsm_t;

/* Important: The state handler functions with this signature must be reentrant
 */
typedef bool (*SMfsm_p_hndlr_t)(SMfsm_fsm_t* const me, const uint8_t evt);

struct SMfsm_fsm
{
    SMfsm_p_hndlr_t p_statHndlr;
};

/* OPERATIONS
 * ==========
 */

void SMfsm_init(SMfsm_fsm_t* const me, const SMfsm_p_hndlr_t p_statHndlr);

bool SMfsm_sendEvt(SMfsm_fsm_t* const me, const uint8_t evt);

SMfsm_p_hndlr_t SMfsm_getStat(const SMfsm_fsm_t* const me);

void SMfsm_trans(SMfsm_fsm_t* const me, const SMfsm_p_hndlr_t p_statHndlr);

#endif /* SMFSM_H */
