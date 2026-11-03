/*
 * A272680  Smallest square that begins with n (in binary).
 * https://oeis.org/A272680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest square whose binary representation begins with
 *            the binary representation of n. Algorithm: for k = 0,1,2,...
 *            consider the interval [ n << k , (n + 1) << k - 1 ]. For each
 *            interval compute the smallest square ≥ left end
 *            (ceil(sqrt(left))²). The first interval containing such a
 *            squar...
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 2780, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9184 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    seq_t low  = (seq_t)n;   /* left end of current interval */
    seq_t high = low;        /* right end of current interval (k = 0) */

    for (;;)
    {
        /* floor sqrt of low */
        seq_t s = isqrt_seq(low);
        if (s * s < low)   /* ceil(sqrt(low)) */
            ++s;
        seq_t square = s * s;

        if (square <= high)
            return square;

        /* advance to next k: double the interval */
        low  = low  * 2;
        high = high * 2 + 1;
    }
}
