/*
 * A365335  The number of exponentially odd coreful divisors of the square
 *          root of the largest square dividing n.
 * https://oeis.org/A365335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = A325837(A000188(n)); Multiplicative: a(p^e) =
 *            floor((e+2)/4)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Computes a(n) for 1‑based index n. */
seq_t a(idx_t n)
{
    /* 0‑based n: n is already >0 because offset is 1. */
    if (n == 1)
        return (seq_t)1;

    /* Factorization of n.  64 primes are enough for 64‑bit n. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t res = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = ex[i];
        /* Only primes with exponent >=2 contribute. */
        if (e < 2)
            continue;

        /* e_m = floor(e/2) * 2  (largest square divisor exponent) */
        uint64_t f = e / 2;              /* f = e_m / 2   */
        seq_t contrib = (seq_t)((f + 1) / 2);
        res *= contrib;
    }

    return res;
}
