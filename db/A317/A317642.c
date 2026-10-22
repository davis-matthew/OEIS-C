/*
 * A317642  Expansion of theta_3(q^2)*theta_3(q^5), where theta_3() is the
 *          Jacobi theta function.
 * https://oeis.org/A317642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (x,y) to 2*x^2 + 5*y^2 = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for unsigned 64-bit values */
static idx_t isqrt_u64(idx_t v) {
    idx_t r = 0;
    idx_t bit = (idx_t)1ULL << 62;   /* largest power of 4 <= max 64-bit value */
    while (bit) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t max_y = isqrt_u64(n / 5);
    for (idx_t y = 0; y <= max_y; ++y) {
        idx_t t = n - 5 * y * y;
        if (t & 1)            /* t must be even */
            continue;
        idx_t x2 = t >> 1;
        idx_t s = isqrt_u64(x2);
        if (s * s == x2) {    /* perfect square */
            seq_t cx = (s == 0) ? 1 : 2;   /* number of x solutions for this y */
            if (y == 0)
                result += cx;
            else
                result += 2 * cx;         /* two signs for y */
        }
    }
    return result;
}
