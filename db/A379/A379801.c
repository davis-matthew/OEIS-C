/*
 * A379801  Nearest integer to n*(1+sqrt(6))/2.
 * https://oeis.org/A379801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to n*(1+sqrt(6))/2
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 5557, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 9450 (2026-11-01)
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
    seq_t n_seq = (seq_t)n;
    /* floor(n * sqrt(6)) = floor( sqrt(6 * n^2) ) */
    seq_t m = n_seq * n_seq;
    m = m * (seq_t)6;
    seq_t s = isqrt_seq(m);
    /* a(n) = (n + floor(n*sqrt(6)) + 1) / 2  */
    return (n_seq + s + 1) / (seq_t)2;
}
