/*
 * A277184  E.g.f. A(x) satisfies A(x) = x*exp(A(x) - A(x)^2) + A(x)^2.
 * https://oeis.org/A277184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} Catalan(k-1) * n^(n-k) * k! * binomial(n-1,
 *            k-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case explicitly (offset starts at 1). */
    if (n == 0) return 0;

    seq_t n_val = (seq_t)n;

    /* Compute n^(n-1) by repeated multiplication. */
    seq_t n_pow = 1;
    for (idx_t i = 1; i < n; ++i)
        n_pow *= n_val;

    seq_t sum   = 0;
    seq_t cat   = 1;   /* Catalan(0) */
    seq_t fact  = 1;   /* 1! initially */
    seq_t binom = 1;   /* binomial(n-1, 0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* term = Cat(k-1) * n^(n-k) * k! * C(n-1, k-1) */
        seq_t term = cat * n_pow * fact * binom;
        sum += term;

        if (k == n) break;        /* avoid extra updates after last term */

        /* Update for the next k+1 */
        n_pow /= n_val;                           /* n^(n-(k+1)) */
        binom = binom * (n_val - (seq_t)k) / (seq_t)k;   /* C(n-1, k) */
        fact *= (seq_t)(k + 1);                   /* (k+1)! */
        seq_t mult = (seq_t)(4 * k - 2);          /* 2*(2k-1) = 4k-2 */
        cat = cat * mult / (seq_t)(k + 1);        /* Catalan(k) */
    }

    return sum;
}
