/*
 * A377742  E.g.f. satisfies A(x) = exp(x) / (1 - x * A(x))^2.
 * https://oeis.org/A377742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+1)^(n-k-1) * binomial(3*k+1,k) /
 *            (n-k)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation evaluates the sum term‑by‑term, computing
   the binomial coefficient iteratively and using the routine
   seq_pow from the utils library to raise (k+1) to the required
   exponent.  A special “last” term handles the case k = n, where
   the exponent would be negative.  The code works for any
   integer type seq_t supplied by the build, including
   8/16/32/64/128‑bit unsigned integers and arbitrary precision
   types. */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Use a 0‑based index as per OEIS offset. */
    seq_t result = 0;
    /* b = binomial(3*k+1, k), starting with k = 0 */
    seq_t b = 1;
    /* falling = n*(n-1)*...*(n-k+1), starting with k = 0
       (empty product = 1). */
    seq_t falling = 1;

    /* Main loop over k = 0 .. n-1. */
    for (idx_t k = 0; k < n; ++k)
    {
        /* (k+1)^(n-k-1) */
        seq_t pow_term = seq_pow((seq_t)(k + 1), n - k - 1);

        /* term = binomial(3*k+1,k) * pow_term * falling */
        seq_t term = b * pow_term;
        term *= falling;
        result += term;

        /* Update binomial for next k using
           B_(k+1) = B_k * (3k+4)(3k+3)(3k+2) / [(k+1)(2k+3)(2k+2)] */
        idx_t kk = k;
        seq_t num = (seq_t)(3 * kk + 4);
        num *= (seq_t)(3 * kk + 3);
        num *= (seq_t)(3 * kk + 2);
        seq_t den = (seq_t)(kk + 1);
        den *= (seq_t)(2 * kk + 3);
        den *= (seq_t)(2 * kk + 2);
        b = b * num / den;

        /* Update falling product for next k */
        falling *= (seq_t)(n - kk);
    }

    /* Handle the last term (k = n).  Here the exponent is -1,
       i.e. division by (n+1). */
    seq_t last_term = b * falling / (seq_t)(n + 1);
    result += last_term;

    return result;
}
