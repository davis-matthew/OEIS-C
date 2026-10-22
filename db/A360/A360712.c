/*
 * A360712  Expansion of Sum_{k>0} (k * x * (1 + k*x^k))^k.
 * https://oeis.org/A360712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(d + n/d - 1) * binomial(d, n/d -1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   binomial_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial_seq.h"
/* Computes a(n) for any seq_t and idx_t. */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1u) result *= base;
        base *= base;
        exp >>= 1u;
    }
    return result;
}

static seq_t term_for_divisor(idx_t d, idx_t n) {
    idx_t md   = n / d;                    /* n/d */
    idx_t exp  = d + md - 1;               /* exponent */
    seq_t pow_term = pow_seq((seq_t)d, exp);
    idx_t k = md - 1;
    seq_t bin_term = 0;
    if (k <= d) bin_term = binomial_seq(d, k);
    return pow_term * bin_term;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {    /* iterate divisors up to sqrt(n) */
        if (n % d == 0) {
            sum += term_for_divisor(d, n);
            idx_t e = n / d;
            if (e != d)
                sum += term_for_divisor(e, n);
        }
    }
    return sum;
}
