/*
 * A377712  a(n) is the least divisor of n with as many decimal digits as n.
 * https://oeis.org/A377712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = least divisor of n with exactly the same number of
 *            decimal digits as n. That is, a(n) = min{ d | d divides n and
 *            digits(d) = digits(n) }
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root of n (floor(sqrt(n))) – works for all 64‑bit values */
static inline idx_t isqrt_u64(idx_t n) {
    if (n == 0 || n == 1) return n;
    idx_t x = n;
    for (;;) {
        idx_t y = (x + n / x) / 2;
        if (y >= x) break;
        x = y;
    }
    return x;
}

/* number of decimal digits of an unsigned 64‑bit integer */
static inline int digits_of_uint64(idx_t n) {
    int d = 0;
    do {
        ++d;
        n /= 10;
    } while (n);
    return d;
}

seq_t a(idx_t n) {
    /* find the required number of digits */
    int dig = digits_of_uint64(n);
    /* lower bound for a divisor with 'dig' digits */
    seq_t low = 1;
    for (int i = 1; i < dig; ++i) low *= 10;

    seq_t best = 0;               /* 0 means not yet found */
    idx_t limit = isqrt_u64(n);

    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            /* candidate divisor d */
            if (d >= low) {
                seq_t cand = (seq_t)d;
                if (best == 0 || cand < best) best = cand;
            }
            idx_t e = n / d;      /* complementary divisor */
            if (e != d && e >= low) {
                seq_t cand = (seq_t)e;
                if (best == 0 || cand < best) best = cand;
            }
        }
    }
    return best; /* guaranteed to be set: n itself is a valid divisor */
}
