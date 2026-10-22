/*
 * A371320  E.g.f. satisfies A(x) = exp(x) + x^2*A(x)^3.
 * https://oeis.org/A371320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (2*k+1)^(n-2*k-1) *
 *            binomial(3*k,k)/(n-2*k)!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static inline seq_t pow_int(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    seq_t perm = (seq_t)1;          /* perm = n!/(n-2*k)! for current k */
    idx_t sub = n;                  /* sub = n-2*k */

    for (idx_t k = 0; k <= n / 2; ++k)
    {
        /* term = perm * binomial(3*k,k) * (2*k+1)^(sub-1) , with division when sub==0 */
        seq_t term = perm * binomial(3 * k, k);

        if (sub == 0)        /* exponent == -1 ==> divide by (2*k+1) */
            term /= (seq_t)(2 * k + 1);
        else if (sub > 0)    /* exponent >= 0 */
            term *= pow_int((seq_t)(2 * k + 1), sub - 1);

        sum += term;

        /* Prepare for next k */
        if (k < n / 2)
        {
            /* perm = perm * sub * (sub-1)  (since new_sub = sub-2) */
            perm *= (seq_t)sub;
            perm *= (seq_t)(sub - 1);
            sub -= 2;
        }
    }
    return sum;
}
