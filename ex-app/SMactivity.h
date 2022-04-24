/** \file */

#ifndef SMACTIVITY_H
#define SMACTIVITY_H

#include <stdio.h>

/* OPERATIONS
 * ==========
 */

/**
 * \{
 * \brief State machine activities for state entry, exit and internal transition
 *
 * Simply print messages (for demonstration purposes).
 *
 * \param stat State (referenced by single character)
 */
void SMactivity_entry(const char stat);
void SMactivity_exit(const char stat);
void SMactivity_transIntern(const char stat);
/** \} */

/**
 * \brief State machine activity for state transition
 *
 * Simply prints message (for demonstration purposes).
 *
 * \param statSrc Source state of transition (referenced by single character)
 * \param statTarg Target state of transition (referenced by single character)
 */
void SMactivity_trans(const char statSrc, const char statTarg);

/**
 * \brief State machine activity for event ignored by state machine
 *
 * Simply prints message (for demonstration purposes).
 */
void SMactivity_ignore(void);

#endif /* SMACTIVITY_H */
