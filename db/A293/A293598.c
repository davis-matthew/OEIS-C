/*
 * A293598  L.g.f.: Sum_{n>=1} x^((2*n-1)^2) / ( (2*n-1) * (1 -
 *          x^(2*n))^(2*n-1) ).
 * https://oeis.org/A293598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1) * [coefficient of x^(2*n-1) in the series *
 *            Sum_{k>=1} x^((2*k-1)^2) / ((2*k-1)*(1 - x^(2*k))^(2*k-1)) ]. *
 *            The coefficient of x^m is a(n)/m when m = 2*n-1. The
 *            implementation * sums the contributions of all terms (k, r) that
 *            hit exponent m, * using exact integer arithmetic
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 200, u64 n <= 770,
 *            u128 n <= 2050, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* offset is 1; undefined for n=0 */

    seq_t target_exp = (seq_t)(2 * n - 1);   /* exponent m = 2*n-1 */

    seq_t result = 0;

    /* Iterate over source terms indexed by k (called np here) */
    for (idx_t np = 1; ; ++np)
    {
        seq_t odd   = (seq_t)(2 * np - 1);        /* 2*np-1                 */
        seq_t base  = odd * odd;                 /* (2*np-1)^2             */

        if (base > target_exp)
            break;                               /* further terms too large */

        seq_t diff  = target_exp - base;
        seq_t step  = (seq_t)(2 * np);            /* 2*np                    */

        /* Does (target_exp - base) divide by 2*np? */
        if (diff % step != 0)
            continue;

        seq_t k = diff / step;                    /* index r in the expansion */

        /* Compute binomial coefficient C(a_val + k, a_val) where
         * a_val = 2*np - 2.
         */
        seq_t a_val = (seq_t)(2 * np - 2);
        seq_t binom = 1;

        for (idx_t j = 1; j <= k; ++j)
        {
            seq_t numer = a_val + j;              /* (a_val + j)            */
            seq_t g     = gcd((seq_t)j, numer);   /* reduce before multiply */
            numer /= g;
            /* Multiply then divide exactly by j/g */
            binom = binom * numer;
            binom = binom / (j / g);
        }

        /* Compute contribution: binom * target_exp / odd */
        seq_t denom = odd;
        seq_t num   = target_exp;

        /* Reduce fraction step by step to keep intermediate values small */
        seq_t g1 = gcd(num, denom);
        num   /= g1;
        denom /= g1;

        seq_t g2 = gcd(binom, denom);
        binom  /= g2;
        denom  /= g2;

        seq_t g3 = gcd(num, denom);
        num    /= g3;
        denom  /= g3;      /* denom should now be 1 */

        seq_t contrib = binom * num;   /* integer contribution */
        result += contrib;
    }

    return result;
}
