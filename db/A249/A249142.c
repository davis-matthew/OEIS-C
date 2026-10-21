/*
 * A249142  Let k be the difference between the smallest square >= n and n.
 *          Sequence gives difference between the smallest square >= k and k.
 * https://oeis.org/A249142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A068527(A068527(n)) where A068527(m) = ceil(sqrt(m))^2 -
 *            m
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the integer square root floor of an unsigned 64‑bit integer */
static idx_t isqrt(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;          /* 2^32 is an upper bound for √(2^64‑1) */
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq = (uint64_t)mid * mid;   /* fits in 64 bits */

        if (sq == (uint64_t)n)
            return mid;
        if (sq < (uint64_t)n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;   /* floor sqrt */
}

/* A068527: difference between the smallest square ≥ n and n */
static seq_t delta(idx_t n)
{
    idx_t r = isqrt(n);
    if ((uint64_t)r * r < (uint64_t)n)
        r++;

    /* compute r² - n safely in 64‑bit */
    uint64_t sq = (uint64_t)r * r;
    uint64_t k   = sq - (uint64_t)n;

    return (seq_t)k;   /* cast to seq_t; may truncate if k exceeds seq_t */
}

/* Public sequence function */
seq_t a(idx_t n)
{
    seq_t d1 = delta(n);
    seq_t d2 = delta((idx_t)d1);
    return d2;
}
