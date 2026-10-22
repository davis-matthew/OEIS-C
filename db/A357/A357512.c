/*
 * A357512  a(n) = Sum_{k = 0..n} k^5 * binomial(n,k)^2 * binomial(n+k,k)^2.
 * https://oeis.org/A357512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} k^5 * binomial(n,k)^2 * binomial(n+k,k)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for binomial coefficients */
static seq_t *binom_n = NULL;   /* binomial(n,k) for k = 0..n           */
static seq_t *binom_nn = NULL;  /* binomial(n+k,k) for k = 0..n         */
static size_t binom_cap = 0;    /* current size of the two arrays       */

seq_t a(idx_t n)
{
    /* Allocate or expand cache if required */
    size_t needed = (size_t)n + 1;
    if (needed > binom_cap) {
        seq_t *tmp1 = (seq_t *)realloc(binom_n, needed * sizeof(seq_t));
        if (!tmp1) return 0;
        seq_t *tmp2 = (seq_t *)realloc(binom_nn, needed * sizeof(seq_t));
        if (!tmp2) {
            free(tmp1);
            return 0;
        }
        binom_n   = tmp1;
        binom_nn  = tmp2;
        binom_cap = needed;
    }

    /* Compute binomial(n,k) for k = 0..n   (Pascal's triangle) */
    binom_n[0] = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t t = binom_n[k - 1] * (seq_t)(n - (k - 1));
        binom_n[k] = t / (seq_t)k;
    }

    /* Compute binomial(n+k,k) for k = 0..n   */
    binom_nn[0] = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t t = binom_nn[k - 1] * (seq_t)(n + k);
        binom_nn[k] = t / (seq_t)k;
    }

    /* Accumulate the sum */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk  = (seq_t)k;
        seq_t k2  = kk * kk;        /* k^2   */
        seq_t k3  = k2 * kk;        /* k^3   */
        seq_t k4  = k3 * kk;        /* k^4   */
        seq_t k5  = k4 * kk;        /* k^5   */

        seq_t t = binom_n[k] * binom_n[k];   /* binom(n,k)^2 */
        t = t * binom_nn[k] * binom_nn[k];   /* * binom(n+k,k)^2 */
        t = t * k5;                        /* * k^5 */
        sum += t;
    }

    return sum;   /* for k=0 the term is 0, so we start from k=1 */
}
