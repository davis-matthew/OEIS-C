/* num_divisors_idx: the number of divisors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIVISORS_IDX_H
#define OEISC_UTILS_NUM_DIVISORS_IDX_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t num_divisors_idx(idx_t n)
{
    /* Computes the divisor-count tau(n) for n <= 64 (safely). */
    seq_t res = 1;
    idx_t tmp = n;
    /* factor 2 */
    idx_t cnt = 0;
    while ((tmp & 1) == 0) {
        ++cnt;
        tmp >>= 1;
    }
    if (cnt > 0)
        res *= (cnt + 1);

    /* odd factors */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        cnt = 0;
        while (tmp % p == 0) {
            ++cnt;
            tmp /= p;
        }
        if (cnt > 0)
            res *= (cnt + 1);
    }
    if (tmp > 1)        /* remaining prime */
        res *= 2;
    return res;
}

#endif
