/*
 * A367273  a(n) = Sum_{k=0..n} binomial(n, k)^2 * (k - n)^k.
 * https://oeis.org/A367273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} binomial(n, k)^2 * (k - n)^k
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t c   = 1;          /* current binomial coefficient C(n,k) */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* C(n,k) = C(n,k-1) * (n-k+1) / k  */
            c = (c * (seq_t)(n - k + 1)) / (seq_t)k;
        }

        seq_t base = (seq_t)k - (seq_t)n;   /* (k - n) */
        seq_t pow  = big_pow(base, (seq_t)k);

        seq_t term = c * c;     /* binomial^2 */
        term = term * pow;      /* * (k - n)^k */

        sum = sum + term;
    }

    return sum;
}
