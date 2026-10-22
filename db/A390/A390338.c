/*
 * A390338  a(n) = Sum_{k=0..n} binomial(4*n+k-1,n-k).
 * https://oeis.org/A390338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n+k-1, n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Helper: multiplicative binomial with gcd reduction to avoid overflow on
   64‑bit builds.  Works for arbitrary‑precision seq_t as well. */
static seq_t binomial_safe(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;                 /* use smaller k */

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t   num      = n - k + i;          /* numerator term */
        seq_t   num_seq  = (seq_t)num;
        seq_t   g        = gcd(result, (seq_t)i);   /* reduce by gcd */
        result /= g;
        idx_t   denom    = i / (idx_t)g;
        result *= num_seq;
        result /= (seq_t)denom;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t top    = 4 * n + k - 1;
        idx_t bottom = n - k;
        sum += binomial_safe(top, bottom);
    }
    return sum;
}
