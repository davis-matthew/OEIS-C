/*
 * A365033  E.g.f. satisfies A(x) = exp(x * A(x)^2 * (1 + x * A(x))^2).
 * https://oeis.org/A365033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A365033: a(n) = n! * Σ_{k=0..n} (n+k+1)^(k-1) * binomial(2*k, n-k)/k! */
/* Offset: 0 (a(0) = 1) */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

/* Computes a(n) for non‑negative n.  Works for arbitrary‑precision seq_t
   as well as for uint64_t; intermediate values stay within 64‑bit limits
   for all n whose result fits in uint64_t (up to n = 12). */
seq_t a(idx_t n)
{
    /* a(0) = 1 is a special case (k=0 term only). */
    if (n == 0)
        return (seq_t)1;

    /* Compute n! */
    seq_t fact_n = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n *= (seq_t)i;

    /* ratio = n! / k!  (initially for k = 0)  */
    seq_t ratio = fact_n;
    seq_t sum   = 0;

    /* k runs from 1 to n; the k = 0 term is zero except for n = 0. */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* (n + k + 1)^(k - 1)  (base unsigned). */
        seq_t pow_term = big_pow((seq_t)(n + k + 1), (seq_t)(k - 1));

        /* binomial(2k, n-k).  For k > n, binomial would be 0, but k ≤ n. */
        seq_t bin = binomial(2 * k, n - k);

        /* term = ratio * pow_term * bin.  ratio already contains n! / k!. */
        seq_t term = ratio * pow_term;
        term *= bin;

        sum += term;

        /* prepare ratio for next iteration: divide by (k + 1).  The result
           is integral because (k + 1) divides n! / k!. */
        ratio /= (seq_t)(k + 1);
    }

    return sum;
}
