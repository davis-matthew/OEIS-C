/*
 * A379805  Floor of n*(1+sqrt(6))/2.
 * https://oeis.org/A379805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(1+sqrt(6))/2)
 * Range:     u8 n <= 6, u16 n <= 649, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned integer type seq_t.
   Returns floor(sqrt(x)). */
static seq_t int_isqrt(seq_t x) {
    seq_t low = 0;
    seq_t high = x;
    while (low < high) {
        seq_t mid = low + (high - low + 1) / 2;
        /* Avoid overflow by using division. */
        if (mid != 0 && mid > x / mid) {
            high = mid - 1;
        } else {
            low = mid;
        }
    }
    return low;
}

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n_sq = n_seq * n_seq;     /* n^2  */
    seq_t x = (seq_t)6 * n_sq;      /* 6*n^2 */
    seq_t m = int_isqrt(x);         /* floor(n*sqrt(6)) */
    return (n_seq + m) / 2;         /* floor((n + floor(n*sqrt(6)))/2) */
}
