/*
 * A320258  a(n) = n! * [x^n] exp(x*exp(-n*x)).
 * https://oeis.org/A320258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * (-n*k)^(n-k)
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle n = 0 directly (0! * [x^0]exp(x) = 1) */
    if (n == 0) {
        return 1;
    }

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* binomial coefficient C(n,k) */
        seq_t bin = binomial(n, k);

        /* Absolute value of the base: n*k */
        seq_t base_abs = (seq_t)(n * k);

        /* Exponent: n - k */
        idx_t exp = n - k;

        /* Compute (n*k)^(n-k) */
        seq_t pow_val = big_pow(base_abs, exp);

        /* Apply sign of (-n*k)^(n-k) : (-1)^(n-k) */
        if (exp & 1) {
            pow_val = -pow_val;
        }

        /* Term: binomial * pow_val */
        seq_t term = bin * pow_val;

        /* Accumulate */
        sum += term;
    }

    return sum;
}
