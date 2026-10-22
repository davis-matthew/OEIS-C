/*
 * A375932  The largest unitary k-free divisor of n where k = A051903(n) is
 *          the maximum exponent in the prime factorization of n.
 * https://oeis.org/A375932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n / Π_{pᵢ^{eᵢ}||n, eᵢ = max} pᵢ^{eᵢ} (i.e. the product of
 *            those prime powers whose exponent is the maximum exponent in n's
 *            factorisation)
 * Range:     u8 n <= 1027, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Fast return for the trivial case */
    if (n <= 1) return (seq_t)1;   /* a(1)=1 */

    uint64_t nn = (uint64_t)n;          /* original n as 64‑bit integer */

    /* Small static buffer is enough for 64‑bit numbers:
       the maximal number of distinct prime factors < 16 */
    static const size_t MAX_FACTORS = 16;
    uint64_t pr[MAX_FACTORS];
    uint64_t ex[MAX_FACTORS];
    size_t cnt = 0;

    /* Factorise n (pr[i] is prime, ex[i] its exponent) */
    factor_u64(nn, pr, ex, &cnt);

    /* Determine the maximum exponent e_max */
    uint64_t e_max = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] > e_max) e_max = ex[i];

    /* Compute the product of prime powers whose exponent equals e_max.
       All intermediate values fit in 128 bits because the product never
       exceeds n ≤ 2⁶⁴−1. */
    __uint128_t prod128 = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] == e_max) {
            __uint128_t term = 1;
            for (uint64_t j = 0; j < ex[i]; ++j)
                term *= pr[i];
            prod128 *= term;
        }
    }

    uint64_t prod = (uint64_t)prod128;          /* <= nn */
    seq_t result = (seq_t)(nn / prod);          /* a(n) = n / prod */

    return result;
}
