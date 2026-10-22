/*
 * A372833  a(n) is the denominator of Sum_{d|n, d <= sqrt(n)} 1/d.
 * https://oeis.org/A372833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of sum_{d|n, d <= sqrt(n)} 1/d
 * Range:     u8 n <= 259, u16 n <= 9873, u32 n <= 9873, u64 n <= 9873,
 *            u128 n <= 9873, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Integer square root (floor) for unsigned 64‑bit values */
static inline idx_t int_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    seq_t num = 0;
    seq_t den = 1;

    idx_t lim = int_sqrt(n);
    for (idx_t d = 1; d <= lim; ++d)
    {
        if (n % d != 0)
            continue;

        seq_t g  = gcd(den, (seq_t)d);
        seq_t mul = (seq_t)d / g;        /* d / g      */
        seq_t add = den / g;             /* den / g    */

        seq_t new_num = num * mul + add;
        seq_t new_den = den * mul;       /* lcm(den,d) */

        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return den;
}
