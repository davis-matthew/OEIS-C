/*
 * A305152  Expansion of Sum_{k>0} x^(k^2) / (1 + x^k).
 * https://oeis.org/A305152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, d <= sqrt(n)} (-1)^(d + n/d)
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root (floor) for 64‑bit unsigned integers.
   Uses only addition, subtraction, bitwise shifts – no 128‑bit arithmetic. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;                /* current result */
    idx_t bit = 1ULL << 62;     /* 2^62 is the largest power of 4 ≤ 2^64 */

    /* Move the bit down until it is not greater than the remaining n. */
    while (bit > n)
        bit >>= 2;

    /* Build the result one bit at a time. */
    while (bit != 0) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)           /* out‑of‑range index – harmless */
        return 0;

    seq_t sum = 0;
    idx_t sqrt_n = isqrt_u64(n);   /* floor(sqrt(n)) */

    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            idx_t nd = n / d;
            seq_t term = ((d + nd) & 1ULL) ? -1 : 1;
            sum += term;
        }
    }

    return sum;
}
