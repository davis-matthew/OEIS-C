/*
 * A290158  a(n) = n! * [x^n] exp(-n*x)/(1 + LambertW(-x)).
 * https://oeis.org/A290158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-n)^(n-k) * k^k * binomial(n,k)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* binomial part */
        seq_t bin = binomial(n, k);

        /* k^k */
        seq_t pow_k = seq_pow((seq_t)k, k);

        /* n^(n-k) */
        seq_t pow_n = seq_pow((seq_t)n, n - k);

        /* combine */
        seq_t term = seq_mul(bin, pow_k);
        term = seq_mul(term, pow_n);

        /* sign: (-1)^(n-k) */
        if ((n - k) & 1) {
            term = -term;
        }

        sum += term;
    }
    return sum;
}
