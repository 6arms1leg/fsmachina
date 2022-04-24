/** \file */

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

#include "SMfsm.h"

/* Make `main()` unit testable */
#ifndef TEST
#define MAIN main
#else
#define MAIN testable_main
#endif /* TEST */

/* OPERATIONS
 * ==========
 */

/** \brief Program entry point */
int MAIN(void);

#endif /* MAIN_H */
