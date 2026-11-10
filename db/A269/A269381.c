/*
 * A269381  Least monotonic left inverse of A233271.
 * https://oeis.org/A269381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonzero terms of A233271 that are <= n, where
 *            A233271: b(0)=0, b(k+1) = b(k) + 1 + (number of 0's in binary of
 *            b(k))
 * Range:     u8 n <= 731, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 99 terms through n = 7724 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A233271_next(seq_t x)
{
    seq_t zeros = 0;
    seq_t t = x;
    while (t)
    {
        if ((t & 1) == 0)
            zeros++;
        t >>= 1;
    }
    return x + 1 + zeros;
}

seq_t a(idx_t n)
{
    seq_t lim = (seq_t)n;
    seq_t x = 0;
    seq_t count = 0;
    for (;;)
    {
        seq_t nx = A233271_next(x);
        if (nx <= x)      /* overflow in seq_t */
            break;
        if (nx > lim)
            break;
        count++;
        x = nx;
    }
    return count;
}
