/*
 * A289654  Related to number of mesh patterns of length 2 that avoid the
 *          pattern 321.
 * https://oeis.org/A289654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = Catalan(n‑1) – 2 for n ≥ 6, where Catalan(k) = binom(2k,
 *            k) / (k+1). The first five terms are a(1)=1, a(2)=1, a(3)=1,
 *            a(4)=3, a(5)=13
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial( n , k ) multiplicatively.  Collections of
   arithmetic on seq_t are done with the usual operators. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;       /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply by (n - k + i) */
        res = res * (seq_t)(n - k + i);
        /* Divide by i – guaranteed to be divisible */
        res = res / (seq_t)i;
    }
    return res;
}

/* Catalan number C_k = binom(2k, k) / (k+1). */
static seq_t catalan(idx_t k)
{
    seq_t b = binom(2 * k, k);
    return b / ((seq_t)k + 1);
}

seq_t a(idx_t n)
{
    /* First five terms are explicitly defined */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)3;
    if (n == 5) return (seq_t)13;

    /* For n ≥ 6, a(n) = Catalan(n‑1) – 2 */
    idx_t k = n - 1;
    seq_t c = catalan(k);
    return c - (seq_t)2;
}
