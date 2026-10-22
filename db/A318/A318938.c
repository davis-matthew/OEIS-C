/*
 * A318938  If n=0 then 1 otherwise 16*(1+22*A318935(n))*(sum of cubes of odd
 *          divisors of n).
 * https://oeis.org/A318938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n)=16*(1+22*A318935(n))⋅∑_{d|n, d odd} d³
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

/* Count the exponent of 2 dividing n (v₂(n)). */
static idx_t count_v2(idx_t n)
{
    idx_t v = 0;
    while ((n & 1) == 0)
    {
        ++v;
        n >>= 1;
    }
    return v;
}

/* Compute 8^exp as seq_t. */
static seq_t pow8(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= (seq_t)8;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* v₂(n) and the odd part of n */
    idx_t v   = count_v2(n);
    idx_t odd = n >> v; /* n / 2ᵛ */

    /* A318935(n) = (8^(v+1) - 1)/7 */
    seq_t A = (pow8(v + 1) - 1) / 7;

    /* Sum of cubes of odd divisors of n = σ₃(odd) */
    seq_t sum_cubes = sigma_k(odd, 3);

    /* a(n) = 16*(1 + 22*A) * sum_cubes */
    return 16 * (seq_t)(1 + 22 * A) * sum_cubes;
}
