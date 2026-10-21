/*
 * A257480  S(n) = (3 + (3/2)^v(1 + F(4*n - 3))*(1 + F(4*n - 3)))/6, n >= 1,
 *          where F(x) = (3*x + 1)/2^v(3*x + 1) for x odd, and v(y) denotes
 *          the 2-adic valuation of y.
 * https://oeis.org/A257480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3 + (3/2)^v(1 + F(4*n-3))*(1 + F(4*n-3)))/6 where F(x) =
 *            (3*x+1)/2^v(3*x+1) for x odd and v(y) is the 2‑adic valuation of
 *            y
 * Range:     u8 n <= 21, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   pow3v
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3v.h"

static inline seq_t odd_part(seq_t x) {
    /* return the odd part of x (x / 2^v(x)) */
    while ((x & 1) == 0) {
        x >>= 1;
    }
    return x;
}

seq_t a(idx_t n) {
    /* compute S(n) as defined above */
    /* 4*n - 3 is always odd for n >= 1 */
    seq_t x = (seq_t)4 * (seq_t)n - 3;
    seq_t t = 3 * x + 1;          // 3*x + 1
    seq_t F = odd_part(t);        // odd part of 3*x+1
    seq_t m = F + 1;              // 1 + F
    /* compute v2(m) and odd part s2 of m */
    idx_t v2 = 0;
    seq_t tmp = m;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        ++v2;
    }
    seq_t s2 = tmp;               // odd part of m
    seq_t pow3 = pow3v(v2);
    seq_t numerator = 3 + pow3 * s2;
    seq_t result = numerator / 6;
    return result;
}
