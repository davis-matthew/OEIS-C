/*
 * A336336  Squared distance from start of a point moving in a square spiral.
 * https://oeis.org/A336336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A174344(n)^2 + A274923(n)^2 (squared distance from the
 *            origin after n‑1 steps of a square spiral)
 * Range:     u8 n <= 529, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer (floor). */
static uint64_t isqrt_u64(uint64_t x)
{
    if (x < 2) return x;          /* 0 or 1 */
    uint64_t lo = 0, hi = (x >> 1) + 1;  /* hi is guaranteed > sqrt(x) */
    while (lo < hi)
    {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((__uint128_t)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* The step number in the spiral (0‑based).  a(n) is the squared
       distance after n‑1 steps, so we use s = n‑1. */
    idx_t s = n - 1;

    /* Ring index l: the smallest l such that (2*l+1)^2 > s. */
    uint64_t sqrt_s = isqrt_u64(s);
    idx_t l = (idx_t)((sqrt_s + 1) >> 1);   /* l >= 1 for n > 1 */

    /* Previous maximum step index (end of ring l‑1).  It equals (2*l‑1)^2. */
    __uint128_t prev_max128 = (__uint128_t)(2 * l - 1) * (2 * l - 1);
    idx_t prev_max = (idx_t)prev_max128;

    /* Zero‑based offset within the current ring. */
    idx_t offset = s - prev_max;

    int64_t ix, iy;  /* coordinates can be negative */

    if (offset <= (2 * l - 1))
    {
        /* Moving up from (l, -(l-1)) to (l, l). */
        ix = l;
        iy = -(int64_t)(l - 1) + (int64_t)offset;
    }
    else if (offset <= (4 * l - 1))
    {
        /* Moving left from (l, l) to (-l, l). */
        idx_t o2 = offset - (2 * l);
        ix = (int64_t)l - 1 - (int64_t)o2;
        iy = l;
    }
    else if (offset <= (6 * l - 1))
    {
        /* Moving down from (-l, l) to (-l, -l). */
        idx_t o3 = offset - (4 * l);
        ix = -(int64_t)l;
        iy = (int64_t)l - 1 - (int64_t)o3;
    }
    else
    {
        /* Moving right from (-l, -l) to (l, -l). */
        idx_t o4 = offset - (6 * l);
        ix = -(int64_t)l + 1 + (int64_t)o4;
        iy = -(int64_t)l;
    }

    /* Compute squared distance.  Convert coordinates to absolute positive
       values of type seq_t, then multiply. */
    seq_t ax = ix >= 0 ? (seq_t)ix : (seq_t)(-ix);
    seq_t ay = iy >= 0 ? (seq_t)iy : (seq_t)(-iy);
    return ax * ax + ay * ay;
}
