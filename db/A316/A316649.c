/*
 * A316649  Triangle read by rows in which T(n,k) is the number of length k
 *          chains from (0,0) to (n,n) of the poset [n] X [n] ordered by the
 *          product order, 0 <= k <= 2n, n>=0.
 * https://oeis.org/A316649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 132, u64 n <= 424,
 *            u128 n <= 1561, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A316649:  a(n) = number of length‑k chains from (0,0) to (k,k) in the
   poset [k]×[k] with product order, where k = n - ⌊√n⌋².
   Formula:
       a(n) = Σ_{s=0}^{k} (-1)^s * C(k,s) * C(⌊√n⌋ + k - s - 1, ⌊√n⌋)²
   where C(a,b) = binomial(a,b).  The integer square root is computed
   using the standard sqrt function. */

#include <stdint.h>
#include <math.h>
#include "../utils/binomial.h"

/* integer sqrt using floating point conversion and correction */
static inline idx_t int_sqrt(idx_t n)
{
    idx_t r = (idx_t)sqrt((long double)n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* a(0) = 1 */
        return (seq_t)1;

    idx_t r = int_sqrt(n);     /* row index */
    idx_t k = n - r * r;       /* column index within the row */

    if (k == 0)                /* no chain unless n = 0 */
        return (seq_t)0;

    seq_t pos = 0;             /* sum of even‑s terms */
    seq_t neg = 0;             /* sum of odd‑s terms */

    for (idx_t s = 0; s <= k; ++s) {
        seq_t c1 = binomial(k, s);
        if (c1 == 0) continue;

        idx_t n2 = r + k - s - 1;
        seq_t c2 = (n2 >= r) ? binomial(n2, r) : (seq_t)0;
        if (c2 == 0) continue;

        seq_t term = c1 * c2 * c2;   /* C(k,s) * C(…)² */

        if (s & 1)
            neg += term;
        else
            pos += term;
    }

    return pos - neg;          /* pos ≥ neg for all terms */
}
