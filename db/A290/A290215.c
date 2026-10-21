/*
 * A290215  a(n) = n! * [x^n] -exp(-n*x)*LambertW(-x).
 * https://oeis.org/A290215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0}^{n-1} binom(n,k) * (-n)^k * (n-k)^{n-k-1}
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n = 0 gives the empty sum → 0 */
    seq_t result = (seq_t)0;
    if (n == 0)
        return result;

    seq_t nSeq = (seq_t)n;
    seq_t neg_n = -nSeq;         /* -n as a seq_t */
    seq_t powNeg_n = (seq_t)1;   /* (-n)^k, starting with k = 0 */

    for (idx_t k = 0; k < n; ++k)
    {
        seq_t binom = binomial(n, k);                 /* C(n,k) */
        seq_t n_k   = (seq_t)(n - k);                 /* n - k   */
        seq_t pow_nk = big_pow(n_k,
                               (seq_t)(n - k - 1));     /* (n-k)^{n-k-1} */
        seq_t term = binom * powNeg_n * pow_nk;
        result += term;                               /* accumulate */

        /* prepare (-n)^{k+1} for next iteration */
        powNeg_n *= neg_n;
    }

    return result;
}
