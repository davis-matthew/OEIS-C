/*
 * A380673  Expansion of e.g.f. (1/x) * Series_Reversion( x * (1 - x) * exp(-x
 *          * (1 - x)) ).
 * https://oeis.org/A380673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n+1)^(k-1) * binomial(2*n-2*k, n-k)/k!
 *            This implementation rewrites the sum as a(n) = Σ_{m=0..n}
 *            binomial(2*m, m) * (n+1)^(n-m-1) * (n!)/(n-m)! All intermediate
 *            values remain integers, so only integer arithmetic is used
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the offset 0 separately. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* (n+1) as seq_t for multiplications/divisions. */
    seq_t nplus1 = (seq_t)(n + 1);

    /* Pre‑compute (n+1)^(n-1) needed for the first term of the sum. */
    seq_t pow_part = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i) {
        pow_part *= nplus1;
    }

    seq_t sum = (seq_t)0;   /* accumulator for the sum */
    seq_t ff   = (seq_t)1;   /* falling factorial n!/(n-m)! , start with m=0 */

    /* Sum over m = 0 .. n-1 (the terms with non‑negative exponent). */
    for (idx_t m = 0; m < n; ++m) {
        seq_t c     = binomial(2 * m, m);          /* binomial(2m, m)   */
        seq_t term  = c * ff * pow_part;          /* multiply all parts */
        sum += term;                              /* accumulate */

        /* Update for the next value of m, except after the last iteration. */
        if (m < n - 1) {
            ff        *= (seq_t)(n - m);          /* multiply by (n-m)  */
            pow_part  /= nplus1;                  /* divide by (n+1)    */
        }
    }

    /* Final term corresponding to m = n (exponent -1). */
    seq_t c     = binomial(2 * n, n);              /* binomial(2n, n)   */
    seq_t term  = c * ff;                          /* c * (n!/(n-n)!)   */
    term /= nplus1;                                 /* divide by (n+1)    */
    sum += term;

    return sum;
}
