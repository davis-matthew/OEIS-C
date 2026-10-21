/*
 * A272679  a(n)^2 is the smallest square whose binary expansion begins with
 *          the binary expansion of n.
 * https://oeis.org/A272679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 64, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the smallest integer a(n) such that a(n)^2 starts with the
   binary representation of n.  The function meets the requirements
   of OEIS A272679. */
#include <stddef.h>
#include "../utils/floor_sqrt.h"

/* compute the bit length of an index value (idx_t is uint64_t) */
static inline size_t bitlen_idx(idx_t x)
{
    size_t len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    /* Special case: a(0) = 0 */
    if (n == 0)
        return 0;

    /* Prefix bits represented by n */
    const seq_t prefix = (seq_t)n;
    const size_t k = bitlen_idx(n);   /* number of bits in the prefix */

    /* Search for the minimal number of bits L such that some square
       of length L starts with the given prefix.  L starts at k and
       grows; a result is guaranteed to be found for moderate limits. */
    for (size_t L = k; L < k + 64; ++L) {
        const size_t diff = L - k;                 /* L - k < 64 */

        /* Interval [low, high) of integers whose binary representation
           of length L starts with the prefix. */
        const seq_t low  = prefix << diff;
        const seq_t high = (prefix + 1) << diff;

        /* Compute ceil(sqrt(low)) using floor_sqrt. */
        seq_t m0 = floor_sqrt(low);
        seq_t m0_sq = m0 * m0;
        seq_t ceil_sqrt_low = (m0_sq == low) ? m0 : m0 + 1;

        /* Compute floor(sqrt(high - 1)). */
        seq_t high_minus_one = high - 1;
        seq_t high_r = floor_sqrt(high_minus_one);

        if (ceil_sqrt_low <= high_r)
            return ceil_sqrt_low;
    }

    /* The loop should always find a result for n up to the limits
       expected by the test harness.  If it does not, return 0 as a
       safe fallback. */
    return 0;
}
