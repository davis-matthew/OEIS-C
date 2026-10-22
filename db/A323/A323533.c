/*
 * A323533  a(n) = Product_{k=1..n} (binomial(k-1,5) + binomial(n-k,5)).
 * https://oeis.org/A323533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \prod_{k=1..n} (C(k-1,5)+C(n-k,5))
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

/* Small helper: compute binomial(m,5) for m >= 0. */
/* For m < 5 the value is 0.  The calculation uses only
   integer arithmetic and never overflows for m <= n-1 with n <= 10000
   on the 64‑bit build. */
static seq_t binom5(idx_t m)
{
    if (m < 5) return (seq_t)0;
    seq_t r = 1;
    /* r = r * (m - 5 + j) / j   for j = 1 .. 5   */
    for (idx_t j = 1; j <= 5; ++j)
        r = r * ((seq_t)(m - 5 + j)) / ((seq_t)j);
    return r;
}

/* Computes a(n) as defined.  Works for any n >= 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t term = binom5(k - 1) + binom5(n - k);
        if (term == 0) return (seq_t)0;   /* product becomes zero */
        prod = prod * term;
    }
    return prod;
}
