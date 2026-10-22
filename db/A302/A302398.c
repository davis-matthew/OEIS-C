/*
 * A302398  a(n) = n! * [x^n] 1/(1 + x*exp(n*x)).
 * https://oeis.org/A302398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * k! * (n*k)^(n-k) * binomial(n,k)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;            /* running total */
    seq_t C   = 1;            /* binomial coefficient binomial(n,0) */
    seq_t f   = 1;            /* factorial k! for current k   */

    for (idx_t k = 0; k <= n; ++k) {
        /* sign = (-1)^k */
        seq_t term_sign = (k & 1) ? -1 : 1;

        /* base   = n*k   (cast from idx_t to seq_t) */
        seq_t base = (seq_t)n * (seq_t)k;

        /* exponent = n - k (cast to seq_t) */
        seq_t exp = (seq_t)(n - k);

        /* pow_val = base^exp ; handle 0^0 explicitly */
        seq_t pow_val;
        if (base == 0 && exp == 0)
            pow_val = 1;
        else
            pow_val = big_pow(base, exp);

        /* term = (-1)^k * k! * (n*k)^(n-k) * C(n,k) */
        seq_t term = f;                     /* k!      */
        term = term * pow_val;              /* * base^exp  */
        term = term * C;                    /* * binomial */
        if (term_sign == -1)
            term = -term;

        /* accumulate */
        sum += term;

        /* update factorial k! → (k+1)!  (if not last iteration) */
        if (k < n)
            f = f * (seq_t)(k + 1);

        /* update binomial coefficient C → C(n,k+1)  (if not last) */
        if (k < n) {
            C = C * (seq_t)(n - k);
            C = C / (seq_t)(k + 1);
        }
    }

    return sum;
}
