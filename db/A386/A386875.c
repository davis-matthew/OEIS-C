/*
 * A386875  a(n) is the maximum number of strong sub-tournaments in an
 *          n-tournament.
 * https://oeis.org/A386875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - n*2^((n-1)/2) - 1 if n is odd, a(n) = 2^n -
 *            3*n*2^((n-4)/2) - 1 if n is even
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* handle small indices where formula would misbehave */
    if (n < 4) {
        static const seq_t known[4] = {0, 0, 0, 1};
        return known[n];
    }

    seq_t pow_n = pow2(n);

    if (n & 1) {                /* n is odd */
        seq_t pow_half = pow2((n - 1) / 2);
        seq_t term = (seq_t)n * pow_half;
        return pow_n - term - 1;
    } else {                    /* n is even */
        seq_t pow_half = pow2((n - 4) / 2);
        seq_t term = (seq_t)(3 * n) * pow_half;
        return pow_n - term - 1;
    }
}
