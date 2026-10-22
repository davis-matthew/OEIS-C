/*
 * A387200  Coefficient of x^n in the expansion of (1+x^4+x^5)^n.
 * https://oeis.org/A387200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=0..⌊n/4⌋} \binom{n}{k} * \binom{k}{\,n-4k\,}
 *            (terms with n-4k<0 or n-4k>k are omitted)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 37, u64 n <= 69, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
   Helper: binomial coefficient via multiplicative formula
   Returns binom(n,k) as seq_t; returns 0 if k>n.
   ------------------------------------------------------------------ */
static inline seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i);  /* multiply first */
        res = res / (seq_t)i;            /* divide next   */
    }
    return res;
}

/* ------------------------------------------------------------------
   Main function: coefficient of x^n in (1+x^4+x^5)^n
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    seq_t sum   = 0;
    idx_t kmax  = n / 4;              /* largest possible k */

    for (idx_t k = 0; k <= kmax; ++k)
    {
        idx_t r = n - 4 * k;          /* exponent of the x^5 terms */
        if (r > k) continue;          /* binom(k,r) would be 0 */

        seq_t b1 = binom_idx(n, k);   /* binom(n,k)   */
        seq_t b2 = binom_idx(k, r);   /* binom(k,r)   */

        sum += b1 * b2;               /* accumulate result */
    }
    return sum;
}
