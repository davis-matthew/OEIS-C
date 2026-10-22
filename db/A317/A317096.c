/*
 * A317096  Expansion of e.g.f. ((1 - x)/(1 - 2*x))*exp(x/(x - 1)).
 * https://oeis.org/A317096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A317096: a(n) = Σ_{k=0..n} binomial(n-1,k-1)*A000166(k)*n!/k!
 *  (non-consecutive sum; a(0)=1, a(1)=0).  Implements an O(n) algorithm
 *  using incremental binomial, derangement, and factorial‑ratio
 *  recurrences.  Works for any seq_t (fixed‑size or big‑int) and
 *  any idx_t.  No allocation is needed.  All integer literals are
 *  cast to seq_t where appropriate.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Initial values for the recurrence */
    seq_t sum   = (seq_t)0;                     /* running sum */
    seq_t binom = (seq_t)(n - (seq_t)1);        /* C(n-1,1) */
    /* prod = n! / 2!  →  product of j from 3 to n (empty product = 1) */
    seq_t prod = (seq_t)1;
    for (idx_t j = 3; j <= n; ++j)
        prod *= (seq_t)j;                      /* mul small integer */

    /* Derangements: use two previous values to avoid an array */
    seq_t der_prevprev = (seq_t)1; /* der[0] */
    seq_t der_prev     = (seq_t)0; /* der[1] */

    for (idx_t k = 2; k <= n; ++k)
    {
        /* Current derangement */
        seq_t der_cur = (seq_t)(k - (seq_t)1) * (der_prev + der_prevprev);

        /* Add the term binom * der_cur * prod */
        seq_t term = binom * der_cur;
        term *= prod;
        sum += term;

        /* Update for the next iteration when k < n */
        if (k < n)
        {
            /* binom_{k+1} = binom_k * (n - k) / k */
            binom = binom * (seq_t)(n - k) / (seq_t)k;
            /* prod_{k+1} = prod_k / (k + 1) */
            prod = prod / (seq_t)(k + 1);
        }

        /* Shift derangement values */
        der_prevprev = der_prev;
        der_prev     = der_cur;
    }

    return sum;
}
