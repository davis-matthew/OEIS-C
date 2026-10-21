/*
 * A249441  a(n) is the smallest prime whose square divides at least one entry
 *          in the n-th row of Pascal's triangle, or 0 if there is no such
 *          prime.
 * https://oeis.org/A249441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime whose square divides at least one entry in
 *            the n-th row of Pascal's triangle, or 0 if none
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   base3_carries
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/base3_carries.h"

seq_t a(idx_t n)
{
    /* No prime square can divide any entry of row 0,1,2,3. */
    if (n < 4) return 0;

    /* --- check prime 2 --- */
    unsigned long long pop_n = __builtin_popcountll((unsigned long long)n);
    for (idx_t k = 1; k < n; ++k) {
        unsigned long long pop_k   = __builtin_popcountll((unsigned long long)k);
        unsigned long long pop_nk  = __builtin_popcountll((unsigned long long)(n - k));
        if (pop_k + pop_nk >= pop_n + 2) return 2;
    }

    /* --- check prime 3 --- */
    for (idx_t k = 1; k < n; ++k) {
        if (base3_carries(k, n - k) >= 2) return 3;
    }

    /* No prime square divides any binomial coefficient in this row. */
    return 0;
}
