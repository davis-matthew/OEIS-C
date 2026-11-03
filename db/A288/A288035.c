/*
 * A288035  Number of (undirected) paths in the complete bipartite graph
 *          K_n,n.
 * https://oeis.org/A288035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!^2 * Sum_{k=0..n-1} (1 + k)/(k!^2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements recurrence: term_{k+1} = term_k * (k+2)/(k+1)^3,
   starting from term_0 = (n!)^2.  Summation over k produces a(n). */
#include <stdint.h>

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* defined only for n ≥ 1, but safe */
    seq_t fact = factorial(n);      /* n! */
    seq_t term = fact * fact;       /* term_0 = n!^2 */
    seq_t sum  = term;              /* initialize sum */

    for (idx_t k = 0; k + 1 < n; ++k)   /* compute terms for k=1..n-1 */
    {
        /* term_{k+1} = term_k * (k+2) / (k+1)^3  */
        seq_t num = term * (seq_t)(k + 2);
        seq_t den = (seq_t)(k + 1);
        den = den * den * den;       /* (k+1)^3 */
        term = num / den;            /* exact division */
        sum += term;
    }
    return sum;
}
