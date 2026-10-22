/*
 * A307947  Coefficient of x^n in 1/(n+1) * (1 + x - n*x^2)^(n+1).
 * https://oeis.org/A307947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (-n)^k * binomial(n,k) *
 *            binomial(n-k,k)/(k+1)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 12, i64 n <= 21, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
extern seq_t binomial(idx_t n, idx_t k);
extern seq_t catalan(idx_t k);

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow = 1;          /* (-n)^k  */
    seq_t binom = 1;        /* binomial(n,0)   */
    seq_t cat = 1;          /* Catalan(0)      */
    seq_t nseq = (seq_t)n;

    idx_t maxk = n / 2;     /* floor(n/2) */
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term = pow * binom;
        term = term * cat;
        sum += term;

        if (k < maxk) {
            /* update power (-n)^(k+1) */
            pow = pow * (-(nseq));

            /* update binom to binomial(n, 2(k+1)) */
            idx_t r = 2 * k;
            binom = binom * ((seq_t)n - (seq_t)r);
            binom = binom / ((seq_t)r + 1);
            binom = binom * ((seq_t)n - (seq_t)(r + 1));
            binom = binom / ((seq_t)r + 2);

            /* update catalan to Catalan(k+1) */
            cat = cat * (seq_t)(2 * (2 * k + 1));
            cat = cat / ((seq_t)k + 2);
        }
    }
    return sum;
}
