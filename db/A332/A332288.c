/*
 * A332288  Number of unimodal permutations of the multiset of prime indices
 *          of n.
 * https://oeis.org/A332288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(n) / (e_max(n) + 1) where τ(n) is the number of
 *            positive divisors of n and e_max(n) is the exponent of the
 *            largest prime divisor of n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS sequence A332288. */
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Special case: a(1) = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Allocate stack arrays for prime factors and exponents. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;

    /* Factorise n. */
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Compute τ(n) = ∏ (e_i + 1). */
    seq_t tau = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (seq_t)(ex[i] + 1);

    /* Find the largest prime divisor and its exponent. */
    size_t idx_max = 0;
    for (size_t i = 1; i < cnt; ++i)
        if (pr[i] > pr[idx_max])
            idx_max = i;
    uint64_t e_max = ex[idx_max];

    /* Return τ(n) divided by (e_max + 1). */
    return tau / (seq_t)(e_max + 1);
}
