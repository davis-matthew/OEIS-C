/*
 * A382512  Expansion of Sum_{p prime} x^p / (1 - x^p)^p.
 * https://oeis.org/A382512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} binomial(n/p+p-2, p-1)
 * Range:     u8 n <= 24, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;          /* a(1) = 0, handle n=0 safely */

    idx_t orig = n;
    seq_t sum = (seq_t)0;
    idx_t m = orig;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            /* prime divisor p found */
            idx_t n_over_p = orig / p;
            idx_t a_val = n_over_p + p - 2;   /* n/p + p - 2 */
            idx_t b_val = p - 1;              /* p - 1 */
            sum += binomial(a_val, b_val);

            /* strip all instances of p from m */
            while (m % p == 0) m /= p;
        }
    }

    /* if m > 1, it is a prime factor larger than sqrt(orig) */
    if (m > 1) {
        idx_t p = m;
        idx_t n_over_p = orig / p;
        idx_t a_val = n_over_p + p - 2;
        idx_t b_val = p - 1;
        sum += binomial(a_val, b_val);
    }

    return sum;
}
