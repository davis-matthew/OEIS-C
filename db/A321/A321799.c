/*
 * A321799  G.f. A(x) satisfies A(x) = 1/(1 - x/(1 - x*A(x))^5).
 * https://oeis.org/A321799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} ( C(n,k) / (n‑k+1) ) * C(n+4k‑1 , n‑k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 45,
 *            big any n
 * Helpers:   binom, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A321799 – offset 0 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binom.h"     /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/seq_gcd.h"   /* seq_t seq_gcd(seq_t a, seq_t b); */

/* Reduce a * b / den exactly by canceling gcd factors with den. */
static inline seq_t mul_div_exact(seq_t a, seq_t b, seq_t den)
{
    /* cancel gcd(a, den) */
    seq_t g = seq_gcd(a, den);
    a /= g;
    den /= g;
    /* cancel gcd(b, den) */
    g = seq_gcd(b, den);
    b /= g;
    den /= g;                 /* den should now be 1 */
    return a * b;             /* exact integer division has been performed */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* a(0) = 1 */

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t c1 = binom(n, k);                           /* C(n,k) */

        idx_t n_plus = n + 4 * k - 1;
        seq_t c2 = binom(n_plus, n - k);                 /* C(n+4k‑1, n‑k) */

        seq_t denom = (seq_t)(n - k + 1);                 /* (n‑k+1) */

        seq_t term = mul_div_exact(c1, c2, denom);
        sum += term;
    }
    return sum;
}
