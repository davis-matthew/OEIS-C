/* is_squarefree_idx: whether n is squarefree.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_SQUAREFREE_IDX_H
#define OEISC_UTILS_IS_SQUAREFREE_IDX_H
#include <stdbool.h>
#include <stdint.h>

/* Test if an integer x (idx_t) is square‑free by trial division. */
static inline bool is_squarefree_idx(idx_t x)
{
    if (x < 2) return true;

    /* check factor 2 separately */
    if (x % 2 == 0) {
        x /= 2;
        if (x % 2 == 0) return false;
        while (x % 2 == 0) x /= 2;
    }

    /* check odd factors */
    for (idx_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            x /= p;
            if (x % p == 0) return false;
            while (x % p == 0) x /= p;
        }
    }
    return true;
}

#endif
