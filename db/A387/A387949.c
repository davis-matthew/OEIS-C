/*
 * A387949  E.g.f. A(x) satisfies A(x) = exp( x^2*A(x)^2 / (1 - x*A(x))^3 ).
 * https://oeis.org/A387949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..floor(n/2)} (n+1)^(k-1) * *
 *            binomial(n+k-1, n-2*k) / k!; a(0)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   fact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fact.h"

/* multiplicative binomial coefficient */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t r = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        r = r * (seq_t)(n - k + i);   /* multiply by numerator */
        r = r / (seq_t)i;             /* divide by denominator (exact) */
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;   /* a(0) = 1 */

    seq_t nfact = fact(n);                /* n! */
    idx_t maxk = n / (idx_t)2;            /* floor(n/2) */

    seq_t sum   = (seq_t)0;
    seq_t power = (seq_t)1;                /* (n+1)^(k-1) */
    seq_t factk = (seq_t)1;                /* k! , starts at 1 for k=1 */

    for (idx_t k = (idx_t)1; k <= maxk; ++k)
    {
        if (k > (idx_t)1)
        {
            factk = factk * (seq_t)k;     /* update k! */
            power = power * (seq_t)(n + (idx_t)1); /* update power */
        }
        seq_t nf_div = nfact / factk;    /* n! / k!  (exact division) */
        seq_t bin = binom(n + k - (idx_t)1, n - (idx_t)2 * k);
        seq_t term = nf_div * power;
        term = term * bin;
        sum += term;
    }
    return sum;
}
