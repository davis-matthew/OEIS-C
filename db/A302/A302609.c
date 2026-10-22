/*
 * A302609  a(n) = n! * [x^n] exp(n*x)*arctanh(x).
 * https://oeis.org/A302609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A302609
   Formula: a(n) = n! * [x^n] exp(n*x) * arctanh(x)
            = sum_{k=1..n, k odd} binomial(n,k) * (k-1)! * n^{n-k}
            = sum_{r odd, 1<=r<=n} (n!/(n-r)!) * n^{n-r} / r
   Offset: 0
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* compute n^n, will be divided down as r grows */
    seq_t pow_rem = 1;
    for (idx_t i = 0; i < n; ++i)
        pow_rem *= (seq_t)n;          /* pow_rem = n^n */

    seq_t sum = 0;
    seq_t P = 1;                      /* P = n!/(n-r)! for current r */

    for (idx_t r = 1; r <= n; ++r) {
        P *= (seq_t)(n - r + 1);        /* extend falling factorial */
        pow_rem = pow_rem / (seq_t)n;   /* pow_rem = n^(n-r)          */
        if (r & 1) {                   /* only odd r contribute      */
            seq_t term = P * pow_rem;   /* P * n^(n-r)                */
            term = term / (seq_t)r;     /* divide by r                */
            sum += term;
        }
    }
    return sum;
}
