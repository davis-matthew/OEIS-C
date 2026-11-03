/* num_divisors_idx_to_idx: the number of divisors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIVISORS_IDX_TO_IDX_H
#define OEISC_UTILS_NUM_DIVISORS_IDX_TO_IDX_H
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

/* Count the number of positive divisors of n. */
static idx_t num_divisors_idx_to_idx(idx_t n)
{
    idx_t temp = n;
    idx_t count = 1;
    for (idx_t i = 2; (uint64_t)i * (uint64_t)i <= temp; ++i) {
        if (temp % i == 0) {
            idx_t exp = 0;
            while (temp % i == 0) {
                temp /= i;
                exp++;
            }
            count *= (exp + 1);
        }
    }
    if (temp > 1)    /* remaining prime factor */
        count *= 2;
    return count;
}

#endif
