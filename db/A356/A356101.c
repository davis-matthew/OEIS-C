/*
 * A356101  Intersection of A000201 and A022839.
 * https://oeis.org/A356101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer x that is in A000201 (floor(k*phi))
 *            but not in A022839 (floor(k*sqrt(5))); found by scanning x
 *            upward
 * Range:     u8 n <= 3, u16 n <= 38, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-10)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

static seq_t A356101_low(seq_t x)
{
    /* floor(x*phi) */
    seq_t r = isqrt_seq(5 * x * x);
    return (x + r) / 2;
}

static seq_t A356101_fl(seq_t x)
{
    /* floor(x/sqrt(5)) */
    return isqrt_seq(x * x / 5);
}

seq_t a(idx_t n)
{
    seq_t x = 0;
    idx_t cnt = 0;
    while (cnt < n)
    {
        x++;
        /* x in A000201 iff floor(phi*(x+1)) - floor(phi*x) == 2 */
        if (A356101_low(x + 1) - A356101_low(x) < 2)
            continue;
        /* x in A022839 iff floor((x+1)/sqrt5) > floor(x/sqrt5) */
        if (A356101_fl(x + 1) > A356101_fl(x))
            continue;
        cnt++;
    }
    return x;
}
