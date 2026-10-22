/*
 * A328002  a(n) = 2^n * Sum_{k=0..n} Product_{j=1..k} (2/j)^((-1)^j).
 * https://oeis.org/A328002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * Sum_{k=0..n} Product_{j=1..k} (2/j)^((-1)^j)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence can be expressed as
   a(n) = Sum_{t=0..⌊n/2⌋} 2^{n-2t} * C(2t,t)   (k even)
          + Sum_{t=1..⌊n/2⌋} 2^{n-2t} * t * C(2t,t)   (k odd, t≥1)
   For the last odd term when n is odd the exponent n-2t becomes
   -1; in that case the term equals (t * C(2t,t))/2.
   This avoids any negative intermediate values. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t maxT = n / 2;
    /* up to which t we need to compute C(2t,t).  For n even this equals maxT,
       for n odd we need one extra t (the last odd term). */
    idx_t extended = (n & 1) ? (maxT + 1) : maxT;

    /* B holds C(2t,t) for the current t.  Start with t=0. */
    seq_t B = 1;

    /* Sum starts with the even term for t=0. */
    seq_t sum = pow2(n) * B;   /* pow2 returns 2^exp as seq_t */

    /* Process t = 1 .. maxT (both even and odd terms). */
    for (idx_t t = 1; t <= maxT; ++t) {
        /* Compute C(2t,t) from C(2(t-1),(t-1)). */
        seq_t num = B * (seq_t)(2 * t);
        num *= (seq_t)(2 * t - 1);
        seq_t den = (seq_t)(t * t);
        B = num / den;                 /* exact division */

        /* Even term: 2^{n-2t} * C(2t,t) */
        seq_t term_even = pow2(n - 2 * t) * B;
        sum += term_even;

        /* Odd term: 2^{n-2t} * t * C(2t,t) */
        seq_t term_odd = pow2(n - 2 * t);
        term_odd *= (seq_t)t;
        term_odd *= B;
        sum += term_odd;
    }

    /* If n is odd we have one extra odd term with exponent -1. */
    if (extended > maxT) {
        idx_t t = extended;                /* t = (n+1)/2 */
        /* Compute C(2t,t) for this t. */
        seq_t num = B * (seq_t)(2 * t);
        num *= (seq_t)(2 * t - 1);
        seq_t den = (seq_t)(t * t);
        seq_t Bt = num / den;               /* C(2t,t) */

        /* The term equals (t * C(2t,t)) / 2. */
        seq_t part = (seq_t)t * Bt;
        sum += part / 2;                   /* exact division */
    }

    return sum;
}
