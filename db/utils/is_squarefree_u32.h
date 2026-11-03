/* is_squarefree_u32: whether n is squarefree.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_SQUAREFREE_U32_H
#define OEISC_UTILS_IS_SQUAREFREE_U32_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Check whether an unsigned 32‑bit number is square‑free ----------------- */
static inline bool is_squarefree_u32(uint32_t n)
{
    if (n < 2) return true;
    for (uint32_t i = 2; (uint64_t)i * i <= n; ++i) {
        if (n % i == 0) {
            n /= i;
            if (n % i == 0) return false;   /* i^2 divides original n      */
        }
    }
    return true;
}

#endif
