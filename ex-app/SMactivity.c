/** \file */

#include "SMactivity.h"

#include "strMsg.h"

/* OPERATIONS
 * ==========
 */

void SMactivity_entry(const char stat) {
    (void)printf(APP_PROMPT "State %c:  Entry activity\n", stat);
}

void SMactivity_exit(const char stat) {
    (void)printf(APP_PROMPT "State %c:  Exit activity\n", stat);
}

void SMactivity_trans(const char statSrc, const char statTarg) {
    (void)printf(APP_PROMPT "%c -> %c:  Transition activity\n",
                 statSrc, statTarg);
}

void SMactivity_transIntern(const char stat) {
    (void)printf(APP_PROMPT "%c internal:  Transition activity\n", stat);
}

void SMactivity_ignore(void) {
    (void)puts(APP_PROMPT "Event ignored");
}
