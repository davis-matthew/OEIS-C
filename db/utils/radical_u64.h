/* radical_u64: the product of the distinct primes dividing n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_RADICAL_U64_H
#define OEISC_UTILS_RADICAL_U64_H
#include <stdbool.h>
#include <stdint.h>

/* compute rad(k) for 0 < k ≤ UINT64_MAX */
static seq_t radical_u64(uint64_t k)
{
    if (k <= 1)
        return 1;                /* rad(1) = 1, rad(0) never used in this sequence */

    seq_t result = 1;

    /* factor 2 */
    if (k % 2 == 0) {
        result *= (seq_t)2;
        while (k % 2 == 0)
            k /= 2;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= k; p += 2) {
        if (k % p == 0) {
            result *= (seq_t)p;
            while (k % p == 0)
                k /= p;
        }
    }

    /* remaining prime factor */
    if (k > 1)
        result *= (seq_t)k;

    return result;
}

#endif
