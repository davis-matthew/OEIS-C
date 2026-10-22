/*
 * A362737  E.g.f. satisfies A(x) = exp(x^3 + x / A(x)).
 * https://oeis.org/A362737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/3⌋} ((-n+3k+1)^(n-2k-1)) / (k! *
 *            (n-3k)!) Equivalent: a(n) = Sum_{k=0..⌊n/3⌋} binomial(n,3k) *
 *            ((3k)!/k!) * ((-n+3k+1)^(n-2k-1))
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Integer binary exponentiation (no modulus).  exp is non‑negative. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t cur = base;
    while (exp)
    {
        if (exp & (idx_t)1U) result *= cur;
        cur *= cur;
        exp >>= 1U;
    }
    return result;
}

/* Compute a(n) defined in the OEIS entry.  Works for any seq_t size
   (signed integer types or arbitrary‑precision class). */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 0.  For n = 0 we merely return 1. */
    if (n == 0) return (seq_t)1;

    seq_t total = (seq_t)0;
    idx_t kmax = n / 3;   /* maximum integer k */

    seq_t fact_3k = (seq_t)1;  /* (3k)! for the current k (start with k=0). */
    seq_t fact_k  = (seq_t)1;  /* k!   for the current k (start with k=0). */

    for (idx_t k = 0; k <= kmax; ++k)
    {
        /* Base and exponent for the power term. */
        seq_t base = (seq_t)(-(int64_t)n + 3 * (int64_t)k + 1);
        idx_t exp  = n - 2 * k - 1;            /* n-2k-1 ≥ 0 for every k in the loop. */
        seq_t powv = pow_seq(base, exp);

        /* Binomial coefficient n choose 3k. */
        seq_t bin = binomial(n, 3 * k);

        /* (3k)! / k! gives the remaining factor, computed via the running
           factorials fact_3k and fact_k.  This division is exact. */
        seq_t ratio = fact_3k / fact_k;

        /* Build the term for this k and add it to the running total. */
        seq_t term = bin;
        term *= ratio;
        term *= powv;
        total += term;

        /* Update running factorials for the next k (if any). */
        if (k < kmax)
        {
            /* update k!  → (k+1)! */
            fact_k *= (seq_t)(k + 1);

            /* update (3k)! to (3(k+1))! by multiplying the three new factors */
            idx_t next = 3 * k;
            fact_3k *= (seq_t)(next + 1);
            fact_3k *= (seq_t)(next + 2);
            fact_3k *= (seq_t)(next + 3);
        }
    }
    return total;
}
