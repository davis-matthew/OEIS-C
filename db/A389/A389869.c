/*
 * A389869  E.g.f. A(x) satisfies A(x) = exp( x^3 * A(x) / (1-x) ) / (1-x).
 * https://oeis.org/A389869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/3⌋} (k+1)^(k-1) * binomial(n-k, n-3k) /
 *            k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the identity a(n) = Σ binomial(n-k, n-3k) * (k+1)^(k-1) * (n! / k!) */

#include <stdint.h>
#include "../utils/binomial.h"

/* Small power: base^exp, exp is an idx_t (non‑negative). */
static seq_t pow_small(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t power  = base;
    while (exp > 0) {
        if (exp & 1)
            result *= power;
        exp >>= 1;
        if (exp)
            power *= power;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Compute n! once. */
    seq_t fact_n = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact_n *= (seq_t)i;

    /* ratio holds n! / k! for the current k.  Starts with k = 0. */
    seq_t ratio = fact_n;
    seq_t sum   = 0;

    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        /* (k+1)^(k-1) ; by convention k=0 gives 1. */
        seq_t pow_term;
        if (k == 0)
            pow_term = 1;
        else
            pow_term = pow_small((seq_t)(k + 1), k - 1);

        /* binomial term. */
        seq_t binom = binomial(n - k, n - 3 * k);

        /* term = binom * pow_term * (n! / k!) , where ratio == n! / k!. */
        seq_t term = pow_term * binom;
        term *= ratio;

        sum += term;

        /* Prepare ratio for next k: divide by (k+1) to obtain n!/(k+1)!. */
        if (k + 1 <= n)
            ratio /= (k + 1);
    }

    return sum;
}
