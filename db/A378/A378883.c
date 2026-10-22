/*
 * A378883  G.f. A(x) satisfies A(x) = 1 + x*A(x)^3/(1 - x*A(x)^5).
 * https://oeis.org/A378883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A378883
   G.f.:  A(x) = 1 + x*A(x)^3/(1 - x*A(x)^5)
   Recurrence (Lagrange inversion):
       a(0) = 1
       a(n) = Σ_{k=1..n} C(5n-2k+1, k) * C(n-1, k-1) / (5n-2k+1)   for n>0
   idx_t, seq_t are supplied by the build. */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) using multiplicative formula.
   Handles large intermediate values with __uint128_t where available. */
static seq_t binom_mul(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;                       /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;
#ifdef __SIZEOF_INT128__
        res = (seq_t)(((__uint128_t)res * num) / den);
#else
        res = res * num / den;
#endif
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t term_den = 5 * n - 2 * k + 1;                     /* 5n-2k+1  */
        seq_t bin1 = binom_mul(term_den, k);                    /* C(5n-2k+1, k) */
        seq_t bin2 = binom_mul(n - 1, k - 1);                    /* C(n-1, k-1) */
        seq_t term;
#ifdef __SIZEOF_INT128__
        term = (seq_t)(((__uint128_t)bin1 * bin2) / term_den);
#else
        term = bin1 * bin2 / term_den;
#endif
        sum += term;
    }
    return sum;
}
