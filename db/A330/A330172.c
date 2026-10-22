/*
 * A330172  a(n) = n + floor(nr/t) + floor(ns/t), where r = sqrt(2) - 1, s =
 *          sqrt(2), t = sqrt(2) + 1.
 * https://oeis.org/A330172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*(3-2*sqrt(2))) + floor(n*(2-sqrt(2)))
 * Range:     u8 n <= 5, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* equivalent: a(n) = 6*n - floor(2*n*sqrt(2)) - floor(n*sqrt(2)) - 2 */

#include "../utils/bitlen_seq_t.h"

/* Integer square root: floor(sqrt(x)) for seq_t */
static seq_t isqrt_seq_t(seq_t x)
{
    if (x == 0) return 0;
    seq_t left  = 0;
    seq_t right = (seq_t)1 << ((bitlen_seq_t(x) + 1) / 2);
    while (left < right) {
        seq_t mid   = (left + right) >> 1;
        seq_t mid2  = mid * mid;
        if (mid2 <= x) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left - 1;
}

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;           /* n^2 */
    seq_t t1 = (seq_t)2 * n2;   /* 2·n^2  -> floor(n*sqrt(2)) = isqrt(2*n^2) */
    seq_t t2 = (seq_t)8 * n2;   /* 8·n^2  -> floor(2n*sqrt(2)) = isqrt(8*n^2) */
    seq_t s1 = isqrt_seq_t(t1);
    seq_t s2 = isqrt_seq_t(t2);
    seq_t res = (seq_t)6 * N - s2 - s1 - (seq_t)2;
    return res;
}
