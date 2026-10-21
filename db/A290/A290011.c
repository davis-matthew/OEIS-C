/*
 * A290011  Number of ways to connect n nodes with n+1 edges to form a
 *          2-edge-connected graph.
 * https://oeis.org/A290011
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^2 + 2*n - 18) * n! / 24
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A290011 – number of ways to connect n nodes with n+1 edges
   to form a 2-edge-connected graph. The function returns a(4)=6,
   a(5)=85, a(6)=900, ... for all n >= 4. It uses a simple factorial
   cache to achieve O(n) time per call. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* for n < 4 the formula is not defined – return 0 in that case */
    if (n < 4u)
        return (seq_t)0;

    /* static cache for the last computed factorial */
    static idx_t cached_n = 0;          /* highest n for which we have fact */
    static seq_t fact = 1;             /* fact == cached_n! */

    /* extend factorial cache if needed */
    if (n > cached_n) {
        for (idx_t i = cached_n + 1; i <= n; ++i)
            fact *= (seq_t)i;
        cached_n = n;
    }
    /* recompute downward if n is smaller than cached_n */
    else if (n < cached_n) {
        fact = 1;
        for (idx_t i = 1; i <= n; ++i)
            fact *= (seq_t)i;
        cached_n = n;
    }

    /* (n^2 + 2*n - 18) */
    seq_t coeff = (seq_t)n * (seq_t)n + (seq_t)2 * (seq_t)n - (seq_t)18;

    /* divide factorial by 24 first to keep numbers smaller */
    seq_t fact_div24 = fact / (seq_t)24;

    return coeff * fact_div24;
}
