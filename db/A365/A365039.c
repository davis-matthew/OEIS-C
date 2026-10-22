/*
 * A365039  E.g.f. satisfies A(x) = exp(x * (1 + x)/A(x)^2).
 * https://oeis.org/A365039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} (-2*k+1)^(k-1) * binomial(k,n-k)/k!
 *            (OEIS A365039)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Computes the sequence value a(n) for arbitrary-precision or fixed-size seq_t. */
seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Compute the lower limit of k where binomial(k,n-k) is non-zero
       i.e. k >= ceil(n/2).  For unsigned idx_t, (n+1)/2 gives ceil(n/2). */
    idx_t start_k = (n + 1) / 2;

    seq_t sum = 0;           /* Accumulates the weighted sum */
    seq_t ratio = 1;         /* Holds n! / k! for the current k.  For k = n, ratio = 1. */

    /* Iterate k from n down to start_k, updating ratio each step */
    for (idx_t k = n; ; --k)
    {
        /* Compute binomial(k, n-k) */
        seq_t bin = binomial(k, n - k);
        if (bin != 0)
        {
            /* Compute power (-2*k+1)^(k-1); special-case k==0 */
            seq_t power;
            if (k == 0)
                power = (seq_t)1; /* 1^(-1) is defined as 1 */
            else
                power = big_pow((seq_t)(-2 * (int)k + 1), (seq_t)(k - 1));

            /* term = (n! / k!) * binomial(k, n-k) * power = ratio * bin * power */
            seq_t term = ratio * bin;
            term *= power;
            sum += term;
        }

        /* Stop when we have processed the smallest k */
        if (k == start_k)
            break;

        /* Update ratio for the next lower k: ratio_{k-1} = ratio_k * k */
        ratio *= (seq_t)k;
    }

    return sum;
}
