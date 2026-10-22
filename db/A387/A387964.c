/*
 * A387964  E.g.f. A(x) satisfies A(x) = exp( x^2*A(x)^2 / (1-x*A(x)) ) /
 *          (1-x*A(x)).
 * https://oeis.org/A387964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (n+1)^(k-1) * binomial(2*n -
 *            k, n - 2*k) / k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation note:  The term for k = 0 contains the factor (n+1)^(-1);
    it is handled by dividing the product n! * binomial(2n, n) by (n+1).
    For k ≥ 1 all numbers stay integral when we use the factor
    n! / k! and the power (n+1)^(k-1).  All operations are performed
    with seq_t types; idx_t values are cast to seq_t when used in
    arithmetic.  The binomial function is taken from the utils.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* common values */
    seq_t fact = 1;                /* n!   */
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t bin0   = binomial(2 * n, n);   /* C(2n,n)  */
    seq_t sum    = 0;
    seq_t n_plus = (seq_t)(n + 1);

    /* term for k = 0 : (n! * C(2n,n)) / (n+1) */
    seq_t term0 = fact * bin0;
    term0 /= n_plus;
    sum += term0;

    idx_t maxk = n / 2;
    if (maxk == 0)
        return sum;   /* n = 0 or 1 already handled */

    /* variables for k >= 1 */
    seq_t C_curr   = bin0;     /* current binomial C(2n - k, n - 2k)           */
    seq_t fact_div = fact;     /* n! / (k-1)!  (starts with k=0 case)          */
    seq_t pow      = 1;        /* (n+1)^(k-1)  :  for k=1 it is 1             */

    for (idx_t k = 1; k <= maxk; ++k) {
        /* update factorial divisor: n! / k! */
        fact_div /= (seq_t)k;

        /* update power (n+1)^(k-1) for the next step */
        if (k >= 2)
            pow *= n_plus;   /* first time when k==2 : pow becomes (n+1)^1 */

        /* update binomial using the ratio
           C(2n-(k-1), n-2(k-1)) -> C(2n-k, n-2k) */
        idx_t a1 = n - 2 * (k - 1);
        idx_t a2 = a1 - 1;                    /* n - 2(k-1) - 1 */
        idx_t b1 = 2 * n - (k - 1);
        idx_t b2 = n + (k - 1) + 1;           /* n + (k-1) + 1 */

        seq_t temp = C_curr * (seq_t)a1;
        temp     *= (seq_t)a2;
        temp     /= ( (seq_t)b1 * (seq_t)b2 );
        C_curr   = temp;

        /* term for this k : (n! / k!) * (n+1)^(k-1) * C_curr */
        seq_t term = fact_div * pow * C_curr;
        sum += term;
    }

    return sum;
}
