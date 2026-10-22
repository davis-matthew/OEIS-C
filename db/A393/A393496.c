/*
 * A393496  a(n) is the GCD of the X and Y-coordinates of the n-th point of
 *          the square spiral.
 * https://oeis.org/A393496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A174344(n), A274923(n)) - coordinates of n‑th point *
 *            of the square spiral
 * Range:     u8 n <= 1314, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Integer square root for unsigned 64‑bit values.
 * Returns the largest x such that x*x <= n.
 */
static idx_t isqrt_u64(idx_t n)
{
    idx_t bit = (idx_t)1 << ((sizeof(idx_t) * 8) / 2);
    idx_t x   = 0;
    while (bit)
    {
        idx_t nxt = x | bit;
        if (nxt <= n / nxt) x = nxt;
        bit >>= 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    /* The origin (0,0) is the first point. */
    if (n == (idx_t)1)
        return (seq_t)0;

    /* Ring index k : smallest k with (2k+1)^2 >= n. */
    idx_t k = (isqrt_u64(n - (idx_t)1) + (idx_t)1) / (idx_t)2;

    /* (2k-1)^2 is the largest index of the previous ring. */
    idx_t prev_side = (idx_t)2 * k - (idx_t)1;
    seq_t max_prev = (seq_t)prev_side * (seq_t)prev_side;

    /* Position of n within its ring (0‑based). */
    idx_t offset = n - (idx_t)max_prev - (idx_t)1;

    /* Length of one side of the ring. */
    idx_t side_len = (idx_t)2 * k;
    idx_t seg     = offset / side_len;                /* 0: up, 1: left,
                                                        2: down, 3: right */
    idx_t pos     = offset % side_len;                /* 0 … side_len-1 */

    long long kk = (long long)k;
    long long x, y;

    switch (seg)
    {
        case 0: /* move up from (k, -(k-1)) */
            x = kk;
            y = -(kk - 1) + (long long)pos;
            break;
        case 1: /* move left from (k-1, k) */
            x = kk - 1 - (long long)pos;
            y = kk;
            break;
        case 2: /* move down from (-k, k-1) */
            x = -kk;
            y = kk - 1 - (long long)pos;
            break;
        default: /* seg == 3, move right from (-k+1, -k) */
            x = -kk + 1 + (long long)pos;
            y = -kk;
            break;
    }

    /* Absolute values before computing GCD. */
    seq_t ax = x >= 0 ? (seq_t)x : (seq_t)(-x);
    seq_t ay = y >= 0 ? (seq_t)y : (seq_t)(-y);

    return gcd(ax, ay);
}
