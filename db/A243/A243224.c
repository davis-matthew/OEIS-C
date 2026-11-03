/*
 * A243224  Number of odd divisors d of n such that d > 1 and d(1+d/3)/2 <= n
 *          <= 3d(d-1)/2.
 * https://oeis.org/A243224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd divisors d > 1 of n such that * d * (1 +
 *            d/3) / 2 <= n <= 3 * d * (d-1) / 2 * This is equivalent to the
 *            integer inequalities: * d * (d + 3) <= 6 * n * 2 * n <= 3 * d *
 *            (d - 1)
 * Range:     u8 n <= 14, u16 n <= 5086, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9049 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdbool.h>

/* Check the two inequalities for a given odd divisor d and n,
   all computed in seq_t to avoid overflow. */
static inline bool valid_odiv(seq_t d, seq_t n)
{
    /* d * (d + 3) <= 6 * n  and 2 * n <= 3 * d * (d - 1) */
    seq_t left1  = d * (d + 3);          /* d*(d+3)          */
    seq_t right1 = (seq_t)6 * n;         /* 6*n              */
    if (left1 > right1) return false;

    seq_t left2  = (seq_t)2 * n;          /* 2*n              */
    seq_t right2 = (seq_t)3 * d * (d - 1);/* 3*d*(d-1)        */
    return left2 <= right2;
}

/* Integer square‑root for unsigned 64‑bit values.
   Returns ⌊√n⌋. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t r = (idx_t)sqrtl((long double)n);
    /* Adjust down if needed */
    while (r > n / r) --r;
    /* Adjust up if needed */
    while ((r + 1) <= n / (r + 1)) ++r;
    return r;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;  /* a(1)=0 */

    seq_t count = 0;
    idx_t root = isqrt_u64(n);

    for (idx_t d = 1; d <= root; ++d)
    {
        if (n % d != 0) continue;

        /* First divisor d */
        if (d > 1 && (d & 1ULL))
        {
            if (valid_odiv((seq_t)d, (seq_t)n))
                ++count;
        }

        idx_t d2 = n / d;
        if (d2 == d) continue;       /* handled already */

        if (d2 > 1 && (d2 & 1ULL))
        {
            if (valid_odiv((seq_t)d2, (seq_t)n))
                ++count;
        }
    }

    return count;
}
