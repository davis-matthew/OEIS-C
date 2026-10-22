/*
 * A354941  a(n) = Sum_{k=0..n} binomial(n,k)^3 * k! * (-2)^(n-k).
 * https://oeis.org/A354941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^3 * k! * (-2)^(n-k)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 9, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute (-2)^n */
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow *= (seq_t)-2;
    }

    seq_t ans   = (seq_t)0;
    seq_t binom = (seq_t)1;   /* C(n,0) */
    seq_t fact  = (seq_t)1;   /* 0! */

    for (idx_t k = 0; k <= n; ++k) {
        /* binom^3 */
        seq_t term = binom * binom;   /* binom^2 */
        term *= binom;                /* binom^3 */

        term *= fact;                 /* * k! */
        term *= pow;                  /* * (-2)^(n-k) */

        ans += term;                  /* accumulate */

        if (k == n) break;            /* last term, no updates needed */

        /* update for next k */
        /* binomial(n,k+1) = binom * (n-k) / (k+1) */
        binom = binom * (seq_t)(n - k);
        binom = binom / (seq_t)(k + 1);

        /* factorial: (k+1)! = fact * (k+1) */
        fact *= (seq_t)(k + 1);

        /* power: (-2)^(n-(k+1)) = pow / (-2) */
        pow /= (seq_t)-2;
    }

    return ans;
}
