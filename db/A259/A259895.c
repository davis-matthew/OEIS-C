/*
 * A259895  Expansion of psi(x^2) * psi(x^3) in powers of x where psi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A259895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of solutions (i,j) of * i(i+1) +
 *            3*j(j+1)/2 = n with i,j >= 0. Equivalently, * a(n) is the
 *            coefficient of x^n in ψ(x^2)·ψ(x^3)
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 103 terms through n = 957 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t nS = (seq_t)n;
    for (idx_t i = 0; ; ++i)
    {
        seq_t t = (seq_t)i * (i + 1);
        if (t > nS) break;

        seq_t rem = nS - t;
        seq_t s = rem * 2;
        if (s % 3 != 0) continue;     /* 3*j(j+1)/2 must be integer */

        seq_t y = s / 3;               /* j(j+1) = y */
        seq_t d = 1 + 4 * y;           /* discriminant: 1 + 4y */
        seq_t sq = floor_sqrt(d);

        if (sq * sq != d) continue;    /* discriminant not a perfect square */
        if ((sq - 1) % 2 != 0) continue;/* j must be integer */

        /* j = (sq - 1) / 2   (value not needed beyond existence) */
        count += 1;
    }
    return count;
}
