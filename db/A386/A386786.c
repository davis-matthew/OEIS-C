/*
 * A386786  a(n) = n^4*sigma_6(n).
 * https://oeis.org/A386786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^4 * sigma_6(n) where sigma_6(n) = sum_{d|n} d^6
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 84, u128 n <= 7120,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n by trial division and computes sigma_6
   multiplicatively using the closed‑form product formula for the divisor
   sigma function.  The resulting value a(n) is returned as a seq_t.       */
#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    uint64_t rem = n;
    seq_t sigma6 = 1;

    /* Factor n by trial division */
    for (uint64_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            unsigned e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            /* term = (p^{6(e+1)} - 1) / (p^6 - 1) */
            seq_t term_num = pow_seq((seq_t)p, 6 * (e + 1)) - 1;
            seq_t term_den = pow_seq((seq_t)p, 6) - 1;
            seq_t term = term_num / term_den;
            sigma6 *= term;
        }
    }
    /* If a prime factor remains */
    if (rem > 1) {
        unsigned e = 1; /* rem is prime, exponent 1 */
        seq_t term_num = pow_seq((seq_t)rem, 6 * (e + 1)) - 1; /* e+1 = 2 */
        seq_t term_den = pow_seq((seq_t)rem, 6) - 1;
        seq_t term = term_num / term_den;
        sigma6 *= term;
    }

    seq_t N = (seq_t)n;
    /* a(n) = n^4 * sigma_6(n) */
    return N * N * N * N * sigma6;
}
