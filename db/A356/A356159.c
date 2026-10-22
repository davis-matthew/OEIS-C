/*
 * A356159  Sum of the prime indices of the smallest number that has the same
 *          prime signature as n.
 * https://oeis.org/A356159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{i=1}^{m} e_i · i, where the exponents {e_i} are the
 *            prime‑factor exponents of the *smallest* number having the same
 *            prime signature as n, arranged in non‑increasing order. For n =
 *            1 the value is 0
 * Range:     u8 n <= 9359, u16 n <= 9359, u32 n <= 9359, u64 n <= 9359,
 *            u128 n <= 9359, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

#define MAX_FACTORS 16   /* enough for n ≤ 10^12, far above the limits */

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n < 2) return (seq_t)0;

    /* Prime factorisation of n */
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Sort exponents in descending order */
    for (size_t i = 0; i + 1 < cnt; ++i)
        for (size_t j = i + 1; j < cnt; ++j)
            if (exps[j] > exps[i]) {
                uint64_t t = exps[i];
                exps[i] = exps[j];
                exps[j] = t;
            }

    /* Compute the weighted sum */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)exps[i] * (seq_t)(i + 1);

    return sum;
}
