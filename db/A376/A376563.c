/*
 * A376563  E.g.f. satisfies A(x) = exp( x*A(x) / (1 - x^3*A(x)^3) ).
 * https://oeis.org/A376563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A376563
   Formula: a(n) = n! * Σ_{k=0..⌊n/3⌋} (n+1)^(n-3k-1) * binomial(n-2k-1,k) / (n-3k)! */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* A(0) = 1 */
    if (n == (idx_t)0) return (seq_t)1;

    seq_t sum = (seq_t)0;
    idx_t maxk = n / 3;                 /* floor(n/3) */

    /* base for powers: n+1 */
    seq_t base = (seq_t)(n + (idx_t)1);
    seq_t pow3 = pow_seq(base, 3);      /* (n+1)^3 */

    /* initial power for k=0: (n+1)^(n-1) */
    seq_t pow = pow_seq(base, (int)(n - (idx_t)1));

    seq_t ratio = (seq_t)1;             /* n!/(n-3k)! for current k */

    for (idx_t k = (idx_t)0; k <= maxk; ++k)
    {
        /* binomial coefficient: C(n-2k-1, k) */
        idx_t a = n - (idx_t)2 * k - (idx_t)1;
        if (a < k) break;              /* binomial zero, can stop here */
        seq_t bin = binomial(a, k);

        /* term = ratio * pow * bin */
        seq_t term = ratio;
        term *= pow;
        term *= bin;
        sum += term;

        /* update for next k (if any) */
        if (k < maxk)
        {
            /* multiply ratio by (n-3k-2)*(n-3k-1)*(n-3k) */
            seq_t f1 = (seq_t)(n - (idx_t)3 * k - (idx_t)2);
            seq_t f2 = (seq_t)(n - (idx_t)3 * k - (idx_t)1);
            seq_t f3 = (seq_t)(n - (idx_t)3 * k);
            ratio *= f1;
            ratio *= f2;
            ratio *= f3;

            /* divide power by (n+1)^3 to reduce exponent by 3 */
            pow /= pow3;
        }
    }

    return sum;
}
