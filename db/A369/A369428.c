/*
 * A369428  The number of exponents in the prime factorization of n that are
 *          squares.
 * https://oeis.org/A369428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of exponents in the prime factorization of n that
 *            are perfect squares
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* integer square root for small values (exponents ≤ 64). */
static inline uint64_t floor_sqrt_small(uint64_t e)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= e)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* undefined in OEIS, guard against bad calls */

    /* buffer large enough for all prime factors of a 64‑bit number */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = ex[i];
        uint64_t r = floor_sqrt_small(e);
        if (r * r == e)
            ++res;
    }
    return res;
}
