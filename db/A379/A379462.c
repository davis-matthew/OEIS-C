/*
 * A379462  a(n) is the total number of paths starting at (0, 0), ending at
 *          (n, 0), consisting of steps (1, 1), (1, 0), (1, -2), and staying
 *          on or above y = -3.
 * https://oeis.org/A379462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 2*binomial(n,3*k) * *
 *            (binomial(3*k+3,k)/(k+2) - binomial(3*k,k-1)/(k+1))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the binom function from utils/binom.h. */

#include <stdint.h>
#include "../utils/binom.h"

/* Euclidean gcd for idx_t. */
static idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    idx_t maxk = n / 3;
    seq_t total = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* binomial(n, 3k) */
        seq_t bin_n_3k = binom(n, 3 * k);

        /* term1: C(3k+3, k)/(k+2) */
        seq_t term1_num = binom(3 * k + 3, k);
        idx_t d1 = k + 2;                /* denominator */

        /* term2: C(3k, k-1)/(k+1), zero when k==0 */
        seq_t term2_num = 0;
        idx_t d2 = k + 1;                /* denominator */
        if (k >= 1) {
            term2_num = binom(3 * k, k - 1);
        }

        /* Common denominator: lcm(d1,d2) */
        idx_t g = gcd_idx(d1, d2);
        seq_t den = ((seq_t)(d1 / g)) * (seq_t)d2;  /* denom fits in seq_t */

        /* Compute difference numerator over common denominator */
        seq_t a1 = term1_num * (seq_t)(d2 / g);
        seq_t a2 = term2_num * (seq_t)(d1 / g);
        seq_t diff = a1 - a2;

        /* term = 2 * bin_n_3k * diff / den */
        seq_t tmp = diff * 2;            /* small multiplier first */
        tmp = tmp * bin_n_3k;
        tmp = tmp / den;

        total += tmp;
    }

    return total;
}
