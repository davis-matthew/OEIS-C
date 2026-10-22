/*
 * A399122  a(n) is the smallest positive index k such that k^2 - k + n has at
 *          least three prime factors counted with multiplicity.
 * https://oeis.org/A399122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 40, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = min{k ≥ 1 : number_of_prime_factors(k^2 - k + n) ≥ 3} */
/* This implementation uses the provided helper sequence_t count_prime_factors.
   It works for any seq_t (byte, word, big integer) and for any idx_t (uint64_t). */

#include <stdbool.h>
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    for (idx_t k = 1; ; ++k)
    {
        /* Compute k^2 - k + n within seq_t to avoid overflow of idx_t. */
        seq_t val = (seq_t)k * ((seq_t)k - 1) + (seq_t)n;

        /* Count total prime factors with multiplicity. */
        seq_t pf = count_prime_factors(val);

        if (pf >= 3)
            return (seq_t)k;    /* k is the smallest index that satisfies the condition */
    }
}
