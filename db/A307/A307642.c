/*
 * A307642  a(n) = n!*Sum_{i=1..n} (Sum_{j=1..i} (i/j)).
 * https://oeis.org/A307642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*Sum_{i=1..n} (Sum_{j=1..i} (i/j))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* offset starts at 1, return 0 for n=0 */
    seq_t a_val = 1;               /* a(1) = 1 */
    seq_t fact   = 1;              /* (n-1)!   for n=1 => 1! */
    seq_t F      = 1;              /* F(1) = 1!*H_1 = 1 */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t nk = (seq_t)k;          /* cast index to seq_t */

        /* F_k = k*F_{k-1} + (k-1)! */
        seq_t newF = nk * F + fact;

        /* fact_k = k!   (used as (k-1)! for next iteration) */
        seq_t newFact = fact * nk;

        /* a_k = k*a_{k-1} + k*F_k */
        seq_t newA = nk * a_val + nk * newF;

        a_val = newA;
        F     = newF;
        fact  = newFact;
    }

    return a_val;
}
