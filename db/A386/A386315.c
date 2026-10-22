/*
 * A386315  Number of points in a face-centered cubic lattice that intersect a
 *          sphere of radius n centered on a point in the lattice.
 * https://oeis.org/A386315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A004015(n^2) = 12 * product over odd prime factors p^e of
 *            n where for p mod 8 ∈ {1,3} the factor is p^e, and for p mod 8 ∈
 *            {5,7} the factor is defined by g_0 = 1, g_k = p * g_{k-1} + 2 (k
 *            ≥ 1)
 * Range:     u8 n <= 20, u16 n <= 2974, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* a(0) = 1 */

    /* Strip all powers of two;  a(2*n) = a(n) */
    idx_t m = n;
    while ((m & (idx_t)1) == 0)
        m >>= 1;

    if (m == 1)
        return (seq_t)12;              /* n is a power of two */

    /* Factor the odd part of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)m, primes, exps, &cnt);

    /* Start with the constant factor 12 */
    seq_t prod = (seq_t)12;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if ((p & 7ULL) == 1ULL || (p & 7ULL) == 3ULL) {
            seq_t p_pow = 1;
            for (uint64_t j = 0; j < e; ++j)
                p_pow *= (seq_t)p;
            prod *= p_pow;
        } else { /* p mod 8 == 5 or 7 */
            seq_t g = 1;
            for (uint64_t j = 0; j < e; ++j)
                g = (seq_t)p * g + 2;
            prod *= g;
        }
    }

    return prod;
}
