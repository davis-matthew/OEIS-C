/*
 * A377535  First term of n-th differences of the sequence x^(x-1) for x >= 1.
 * https://oeis.org/A377535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} (-1)^{n-k} C(n,k) (k+1)^k (first term of the
 *            n-th differences of the sequence x^(x-1)). Computes the term
 *            directly using modular binomial updates and fast exponentiation
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t binom = 1;          /* C(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        /* (k+1)^k computed with fast binary exponentiation */
        seq_t term = binom * pow_uint(k + 1, k);
        if ((k & 1) == 0) {
            sum += term;
        } else {
            sum -= term;
        }
        if (k < n) {
            /* Update binom to C(n,k+1) = C(n,k) * (n-k)/(k+1) */
            binom = (binom * (seq_t)(n - k)) / (seq_t)(k + 1);
        }
    }
    /* final sign adjustment */
    if ((n & 1) != 0) {
        sum = -sum;
    }
    return sum;
}
