/*
 * A367272  a(n) = Sum_{k=0..n} binomial(n, k)^2 * k^(n - k).
 * https://oeis.org/A367272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n, k)^2 * k^(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* local fast exponentiation for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t res = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1)
            res *= b;
        b *= b;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t binom = 1;                 /* C(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        /* compute k^(n-k) */
        seq_t k_pow;
        if (k == 0) {
            k_pow = (n == 0) ? 1 : 0;
        } else {
            k_pow = pow_seq((seq_t)k, n - k);
        }

        /* term = binom^2 * k_pow */
        seq_t term = binom * binom;
        term *= k_pow;
        sum += term;

        /* update binom for next k */
        if (k < n) {
            seq_t top    = (seq_t)(n - k);
            seq_t bottom = (seq_t)(k + 1);
            binom = binom * top / bottom;
        }
    }
    return sum;
}
