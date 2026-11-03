/* double_factorial: n!!.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_DOUBLE_FACTORIAL_H
#define OEISC_UTILS_DOUBLE_FACTORIAL_H
#include <stdint.h>

static seq_t double_factorial(idx_t m)
{
    seq_t result = (seq_t)1;
    /* If m == 0, the loop is skipped and 1 is returned, which is correct. */
    idx_t start = (m & (idx_t)1) ? (idx_t)1 : (idx_t)2; /* odd starts at 1, even at 2 */
    for (idx_t k = start; k <= m; k += (idx_t)2)
        result *= (seq_t)k;
    return result;
}

#endif
