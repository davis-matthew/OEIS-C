/* is_squarefree_i64: whether n is squarefree.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_SQUAREFREE_I64_H
#define OEISC_UTILS_IS_SQUAREFREE_I64_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool is_squarefree_i64(long long m) {
    long long p;
    for (p = 2; p * p <= m; p++)
        if (m % (p * p) == 0) return false;
    return true;
}

#endif
