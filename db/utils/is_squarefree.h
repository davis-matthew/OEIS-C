/* is_squarefree: whether n is squarefree.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_SQUAREFREE_H
#define OEISC_UTILS_IS_SQUAREFREE_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Test whether x is square‑free.  Simple trial division works
   because numbers in the sequence are relatively small for the
   required range of n. */
static bool is_squarefree(seq_t x)
{
    for (idx_t d = 2; (seq_t)d * (seq_t)d <= x; ++d) {
        if (x % d == 0) {
            x /= d;
            if (x % d == 0) return false;
        }
    }
    return true;
}

#endif
