/*
 * A344624  a(n) = Sum_{k=1..n} k^c(k), where c(n) is the characteristic
 *          function of squares.
 * https://oeis.org/A344624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt(n))*(floor(sqrt(n)) - 1)*(2*floor(sqrt(n))
 *            + 5)/6
 * Range:     u8 n <= 48, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: floor(sqrt(n)) for 64‑bit unsigned numbers */
static inline idx_t floor_isqrt_u64(idx_t n) {
    idx_t res = 0;
    idx_t bit = 1ULL << 62;      /* highest power of four <= 2^64-1 */
    while (bit > n) bit >>= 2;   /* find largest bit <= n */
    while (bit) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n) {
    /* m = floor(sqrt(n)) */
    idx_t m = floor_isqrt_u64(n);

    /* Compute m*(m-1)*(2*m+5)/6 in a way that keeps intermediate values integer */
    seq_t part = (seq_t)m * (m - 1);   /* m*(m-1) */
    part /= 2;                          /* divide by 2 (exact) */
    part *= ((seq_t)m * 2 + 5);          /* multiply by (2*m+5) */
    part /= 3;                          /* divide by 3 (exact) */

    seq_t result = (seq_t)n + part;     /* final result */
    return result;
}
