/*
 * A302605  a(n) = n! * [x^n] exp(n*x)*arcsin(x).
 * https://oeis.org/A302605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k odd 1..n} binomial(n,k) * ((k-2)!!)² * n^(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the utility that provides the big integer exponentiation
   function big_pow(base, exp). Adjust if the header name differs
   in your build. */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition. */
    if (n == 0u) {
        return 0;
    }

    seq_t n_seq = (seq_t)n;               /* n as a seq_t value */
    seq_t n_pow_full = big_pow(n_seq, (seq_t)n); /* n^n */
    seq_t currPower = n_pow_full / n_seq;        /* n^(n-1) */
    seq_t n_sq = n_seq * n_seq;                /* n^2 for exponent reduction */

    seq_t result = 0;
    seq_t binom = n_seq;          /* binomial(n,1) = n */
    seq_t dfac = 1;               /* (k-2)!! for k=1, (-1)!! = 1 */

    for (idx_t k = 1; k <= n; k += 2) {
        /* term = binom * dfac^2 * currPower */
        seq_t term = binom;
        term = term * dfac;
        term = term * dfac;       /* (k-2)!!² */
        term = term * currPower;
        result += term;

        if (k + 2 > n) { /* stop if no next odd k */
            break;
        }

        /* Update binomial coefficient: binomial(n,k+2) = binomial(n,k)* (n-k)*(n-k-1) / ((k+1)*(k+2)) */
        seq_t num = binom * (seq_t)(n - k) * (seq_t)(n - k - 1);
        seq_t den = (seq_t)(k + 1) * (seq_t)(k + 2);
        binom = num / den;

        /* Update double factorial: (k+2-2)!! = (k-2)!! * k   (k is odd) */
        dfac = dfac * (seq_t)k;

        /* Update power: reduce exponent by two, i.e. divide by n² */
        currPower = currPower / n_sq;
    }

    return result;
}
