/*
 * A330171  a(n) = n + floor(ns/r) + floor(nt/r), where r = sqrt(2) - 1, s =
 *          sqrt(2), t = sqrt(2) + 1.
 * https://oeis.org/A330171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n + floor(n*sqrt(2)) + floor(2*n*sqrt(2))
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* just in case, offset starts at 1 */
    seq_t N = (seq_t)n;
    seq_t n2n    = N * N * (seq_t)2;      /* 2*n*n  → floor(sqrt(2*n*n)) = floor(n*sqrt2)  */
    seq_t nsqrt  = floor_sqrt(n2n);       /* floor(n*sqrt2) */
    seq_t n2sqrt = floor_sqrt((seq_t)8 * N * N); /* 8*n*n → floor(sqrt(8*n*n)) = floor(2*n*sqrt2) */
    return (seq_t)6 * N + nsqrt + n2sqrt;
}
