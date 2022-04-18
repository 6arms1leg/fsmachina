/** \file */

#ifndef SMFSM_H
#define SMFSM_H

/* `"` used intentionally.  This allows the user to override and provide his
   own implementation before falling back to libc. */
#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"
#include "assert.h" /* For sanity checks (Design by Contract) */

/** \brief Pre-defined state machine events */
enum SMfsm_evt {
    SMFSM_ENTRY = 0,
    SMFSM_EXIT,
    SMFSM_APP_EVT_START /**< \brief user-defined events start here */
};

typedef struct SMfsm_fsm SMfsm_fsm_t; /**< \brief Forward decl. due to circular
                                        dep. */

/**
 * \biref State handler function pointer type
 *
 * Functions of this type indicate state of FSM and implement actual FSM
 * behaviour in application layer.
 * Each such function represents a state which handles events by use of
 * `switch (...)` statement.  Trans. guards can be implemented by use of `if
 * (...)` statements within a `switch (...)` case, that handle conditional
 * trans.
 * State handler functions with this signature *must* be reentrant.
 */
typedef bool (*SMfsm_p_statHndlr_t)(SMfsm_fsm_t* const me, const uint8_t evt);

/**
 * \brief FSM base object type ("class")
 *
 * Holds current state (as state handler function) of a FSM.
 * Base class can be derived from in application layer by user to add extended
 * state variables.
 * A derived class is simply another struct with this struct as its first
 * member, called "base".
 */
struct SMfsm_fsm {
    SMfsm_p_statHndlr_t p_statHndlr; /**< Current state of FSM */
};

/* OPERATIONS
 * ==========
 */

/**
 * \brief Initialize FSM
 *
 * \param me Pointer to FSM object
 * \param p_statHndlr State handler function pointer (indicating init. state)
 */
void SMfsm_init(SMfsm_fsm_t* const me, const SMfsm_p_statHndlr_t p_statHndlr);

/**
 * \brief Send event to FSM
 *
 * The sent event is directly (synchronously) "digested" by FSM.
 *
 * \param me Pointer to FSM object
 * \param evt Event to send to FSM (implemented as `enum`s)
 *
 * \return Indicator whether event was handled (`true`) or ignored (`false`)
 */
bool SMfsm_sendEvt(SMfsm_fsm_t* const me, const uint8_t evt);

/**
 * \brief Query current state of FSM
 *
 * \param me Pointer to FSM object
 *
 * \return State handler function pointer (indicating state)
 */
SMfsm_p_statHndlr_t SMfsm_getStat(const SMfsm_fsm_t* const me);

/**
 * \brief Initiate FSM state transition
 *
 * Handles state trans. procedure as follows (according to UML semantics):
 *
 * * Exec. exit activity of old state (implemented in appl. layer by user)
 * * Change from old to new state of FSM
 * * Exec. entry activity of new state (implemented in appl. layer by user)
 *
 * Trans. activity can be implemented in application layer by user but is only
 * exec. from state handler *before* trans., i.e. before state exit, which is
 * not compliant with UML semantics.
 *
 * \param me Pointer to FSM object
 * \param p_statHndlr State handler function pointer (indicating target state)
 */
void SMfsm_trans(SMfsm_fsm_t* const me, const SMfsm_p_statHndlr_t p_statHndlr);

#endif /* SMFSM_H */
