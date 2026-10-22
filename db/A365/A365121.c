/*
 * A365121  G.f. A(x) satisfies A(x) = (1 + x / (1 - x*A(x))^2)^3.
 * https://oeis.org/A365121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*(n-k+1),k) * binomial(n+k-1, n-k)
 *            / (n-k+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute binomial coefficient C(n,k) for 0 ≤ k ≤ n using multiplicative formula.
   Works for all idx_t values, avoiding factorial computation. */
static inline seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;           /* use symmetry C(n,k)=C(n,n-k) */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply by (n - k + i) and divide by i; division is exact */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* Return the n-th term a(n) of OEIS A365121.  Memoizes results in a
   static cache to allow out‑of‑order requests. */
seq_t a(idx_t n)
{
    /* Static cache and its length.  cache_len is the number of
       entries currently stored (i.e., indices 0 .. cache_len-1). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Return cached value if already computed. */
    if (n < cache_len) return cache[n];

    /* Expand cache to size (n+1). */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {          /* allocation failed – fallback to zero */
        free(cache);
        cache = NULL;
        cache_len = 0;
        return 0;
    }
    cache = tmp;

    /* Compute new entries sequentially. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= i; ++k) {
            seq_t term;
            if (k == 0) {
                /* For k==0 the formula reduces to 1 when i==0, otherwise 0. */
                term = (i == 0) ? (seq_t)1 : (seq_t)0;
            } else {
                seq_t c1 = binom_calc(3 * (i - k + 1), k);   /* C(3*(i-k+1), k) */
                seq_t c2 = binom_calc(i + k - 1, i - k);     /* C(i+k-1, i-k) */
                term = c1 * c2;
                term /= (seq_t)(i - k + 1);                  /* division is exact */
            }
            sum += term;
        }
        cache[i] = sum;
    }
    cache_len = new_len;
    return cache[n];
}
