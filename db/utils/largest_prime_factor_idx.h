/* largest_prime_factor_idx: the largest prime factor of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_LARGEST_PRIME_FACTOR_IDX_H
#define OEISC_UTILS_LARGEST_PRIME_FACTOR_IDX_H
#include <stdint.h>

static seq_t largest_prime_factor_idx(idx_t n)
{
    if (n < 2)
        return (seq_t)1;           /* For n = 1, define P(1) = 1 */

    uint64_t m = (uint64_t)n;
    seq_t max_pf = 1;

    /* Handle factor 2 */
    while ((m & 1ULL) == 0) {
        max_pf = 2;
        m >>= 1;
    }

    /* Trial division by odd numbers */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        while (m % p == 0) {
            max_pf = (seq_t)p;
            m /= p;
        }
    }

    /* If remaining m > 1, it's a prime factor */
    if (m > 1)
        max_pf = (seq_t)m;

    return max_pf;
}

#endif
