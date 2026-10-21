/*
 * A249793  G.f.: Sum_{n>=0} x^n/(1-x)^(5*n) * Sum_{k=0..n} C(n,k)^2 * x^k.
 * https://oeis.org/A249793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A249793
   a(n) = Sum_{i=0..n} Sum_{j=0..i} binomial(i,j)^2 * binomial(n+4*i-j-1 , 5*i-1)
   a(0) = 1                     */
/* Header for the binomial helper supplied by the test harness */
#include <stddef.h>
#include <stdint.h>
#include "../utils/binom.h"

/* Compute a single term a(n).  The algorithm runs in O(n²) time
   and uses only integer arithmetic.  It works for any seq_t width
   and produces the truncated result when seq_t is smaller than the
   full integer value. */
static seq_t compute_a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0) return 1;

    seq_t total = 0;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Pre‑compute binomial(i,j) iteratively */
        seq_t bi = 1;                     /* binomial(i,0) */
        /* Pre‑compute the first term of the second binomial */
        idx_t k = 5 * i - 1;              /* fixed lower index */
        idx_t m = n + 4 * i - 1;          /* initial upper index for j = 0 */
        seq_t bj = binom(m, k);           /* binom(m,k) */

        for (idx_t j = 0; j <= i; ++j)
        {
            /* term = binomial(i,j)^2 * binomial(m,k) */
            seq_t term = bi * bi * bj;
            total += term;

            /* Update bi for next j (if any) */
            if (j < i)
            {
                bi = bi * (i - j) / (j + 1);
            }

            /* Update bj for next j (if any) */
            if (j < i)
            {
                /* m decreases by 1 each step */
                bj = bj * (m - k) / m;
                --m;
            }
        }
    }

    return total;
}

/* Public entry point required by the build */
seq_t a(idx_t n)
{
    return compute_a(n);
}
