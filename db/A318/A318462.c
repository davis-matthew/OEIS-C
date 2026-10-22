/*
 * A318462  a(n) = Sum_{d|n, d < n/d} (d XOR n/d) + {square root of n when n
 *          is a square}.
 * https://oeis.org/A318462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = \sum_{d|n,\,d<n/d} (d XOR n/d) +
 *            \begin{cases}\sqrt{n}&\text{if }n\text{ is a perfect
 *            square}\\0&\text{otherwise}\end{cases}
 * Range:     u8 n <= 107, u16 n <= 9753, u32 n <= 9753, u64 n <= 9753,
 *            u128 n <= 9753, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root for uint64_t, implemented with Newton's method */
static inline idx_t isqrt_u64(idx_t n) {
    if (n == 0 || n == 1) return n;
    idx_t x   = n;
    idx_t y   = (x >> 1) + 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n=0, but keep safe */
    seq_t sum = 0;
    idx_t sqrt_n = isqrt_u64(n);
    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            idx_t other = n / d;
            if (d < other) {
                sum += (seq_t)(d ^ other);
            }
        }
    }
    if (sqrt_n * sqrt_n == n) {
        sum += (seq_t)sqrt_n;
    }
    return sum;
}
