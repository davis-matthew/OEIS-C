/*
 * A384688  Runs of t in the range 0 <= t <= k and the same parity as k, for
 *          successive k >= 0.
 * https://oeis.org/A384688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*r+1 if r < s; otherwise a(n) = 2*(r-s), where n = s^2 +
 *            r with s = floor(sqrt(n)) and r = n - s^2
 * Range:     u8 n <= 5001, u16 n <= 5001, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    /* Computes floor(sqrt(n)) for 64‑bit unsigned integers
       without overflow.  Method adapted from Hacker's Delight. */
    if (n < 2) return n;
    idx_t x = n;
    idx_t bit = 1ULL << 62;            /* highest power of 4 <= MAX */
    while (bit > x) bit >>= 2;
    idx_t res = 0;
    while (bit != 0) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t s = isqrt_u64(n);
    idx_t r = n - s * s;
    idx_t a_val;
    if (r < s) {
        a_val = (r << 1) + 1;            /* 2*r + 1 */
    } else {
        a_val = (r - s) << 1;            /* 2*(r - s) */
    }
    return (seq_t)a_val;
}
