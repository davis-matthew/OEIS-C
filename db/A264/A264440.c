/*
 * A264440  Row lengths of the irregular triangle A137510 (number of divisors
 *          d of n with 1 < d < n, or 0 if no such d exists).
 * https://oeis.org/A264440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max(1, d(n)-2) where d(n) is the number of divisors of n.
 *            For n = 1,2,3,... the function returns the row length of the
 *            irregular triangle A137510
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n is at least 1 by the sequence offset.  Use uint64_t for factorization. */
    uint64_t m = (uint64_t)n;

    /* If n == 1, the factor function returns cnt = 0 and the divisor count
       will be 1, so the formula yields 1. */
    /* Factorize m.  The maximum number of distinct prime factors for a
       64‑bit integer is less than 64, so a small static array suffices. */
    static const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;

    factor_u64(m, primes, exps, &cnt);

    /* Compute d(m) = product_i (exps[i] + 1).  Use seq_t for the result. */
    seq_t div_count = 1;
    for (size_t i = 0; i < cnt; ++i)
        div_count = div_count * ((seq_t)(exps[i] + 1));

    /* Return max(1, d(m)-2).  The subtraction and comparison can be done
       in seq_t because (div_count - 2) is always <= div_count and div_count
       itself fits in seq_t for the tested ranges. */
    if (div_count <= (seq_t)2)
        return (seq_t)1;
    else
        return div_count - (seq_t)2;
}
