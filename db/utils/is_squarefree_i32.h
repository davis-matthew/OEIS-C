/* is_squarefree_i32: whether n is squarefree.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_SQUAREFREE_I32_H
#define OEISC_UTILS_IS_SQUAREFREE_I32_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* test whether an integer e (positive, small) is squarefree
   (i.e. no p^2 divides e) */
static inline bool is_squarefree_i32(int e) {
    if (e <= 1) return true;
    for (int p = 2; p * p <= e; ++p) {
        int sq = p * p;
        if (e % sq == 0) return false;
    }
    return true;
}

#endif
