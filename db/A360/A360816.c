/*
 * A360816  Expansion of Sum_{k>=0} (k*x)^(2*k) / (1 - k*x)^(k+1).
 * https://oeis.org/A360816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} k^n * binomial(n-k,k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   binomial_nn
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial_nn.h"

/* Compute b^e for non‑negative e using binary exponentiation.  */
static inline seq_t pow_seq(seq_t b, idx_t e) {
    seq_t r = 1;
    while (e > 0) {
        if (e & 1) r *= b;
        b *= b;
        e >>= 1;
    }
    return r;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term = 0;
        if (k == 0) {
            term = (n == 0) ? 1 : 0;
        } else {
            seq_t p = pow_seq((seq_t)k, n);
            seq_t b = binomial_nn(n - k, k);
            term = p * b;
        }
        sum += term;
    }
    return sum;
}
