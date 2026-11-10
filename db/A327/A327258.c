/*
 * A327258  Positions of 1's in {A327256(n) : n > 0}.
 * https://oeis.org/A327258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A327256(k) = 1, where A327256(k) =
 *            floor(2*k*sqrt(8)) - 2*floor(k*sqrt(8)); computed by a linear
 *            scan over k
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 6651, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9618 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* largest m in [lo, hi] with m*m <= x (assumes lo*lo <= x) */
static seq_t A327258_fsqrt(seq_t x, seq_t lo, seq_t hi)
{
    while (lo < hi)
    {
        seq_t mid = (lo + hi + 1) / 2;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static seq_t A327258_bit(idx_t k)
{
    seq_t sk = (seq_t)k;
    seq_t n2 = sk * sk;
    /* floor(sqrt(32 k^2)) lies in [5k, 6k]; floor(sqrt(8 k^2)) lies in [2k, 3k] */
    seq_t i1 = A327258_fsqrt((seq_t)32 * n2, (seq_t)5 * sk, (seq_t)6 * sk);
    seq_t i2 = A327258_fsqrt((seq_t)8 * n2, (seq_t)2 * sk, (seq_t)3 * sk);
    return i1 - (i2 + i2);
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (count < n)
    {
        k++;
        if (A327258_bit(k) == (seq_t)1)
            count++;
    }
    return (seq_t)k;
}
