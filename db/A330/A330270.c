/*
 * A330270  a(n) is the least nonnegative integer k such that n XOR k is a
 *          square (where XOR denotes the bitwise XOR operator).
 * https://oeis.org/A330270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the least nonnegative k such that n XOR k is a perfect
 *            square
 * Range:     u8 n <= 63, u16 n <= 2821, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(x)) for any unsigned integer type seq_t.  */
static inline seq_t isqrt_seq(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x;
    while (lo <= hi)
    {
        seq_t mid = (lo + hi) >> 1;          /* mid = (lo+hi)/2  */
        seq_t sq  = mid * mid;

        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
        {
            if (mid == 0) break;            /* avoid underflow */
            hi = mid - 1;
        }
    }
    return lo - 1;                         /* floor(sqrt(x)) */
}

/* Sequence a(n) for OEIS A330270. */
seq_t a(idx_t n)
{
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t val = ((seq_t)n) ^ ((seq_t)k);
        seq_t r   = isqrt_seq(val);
        if (r * r == val)
            return (seq_t)k;
    }
    /* Fallback: k = n always works because n XOR n = 0 (a perfect square) */
    return (seq_t)n;
}
