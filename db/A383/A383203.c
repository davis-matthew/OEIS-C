/*
 * A383203  Expansion of e.g.f. f(x) * exp(f(x)), where f(x) = (exp(2*x) -
 *          1)/2.
 * https://oeis.org/A383203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} k * 2^(n-k) * Stirling2(n,k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 0,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ----- cache of already computed a(m) ---------------------------------- */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_upto = 0;

    /* ----- Stirling numbers of the second kind, last computed row --------- */
    static seq_t *S = NULL;
    static size_t S_cap = 0;
    static idx_t S_upto = 0;

    /* ----- powers of two (2^i) -------------------------------------------- */
    static seq_t *pow2 = NULL;
    static size_t pow2_cap = 0;
    static idx_t pow2_upto = 0;

    /* ----- trivial case --------------------------------------------------- */
    if (n == 0)
        return (seq_t)0;

    /* ----- ensure cache array ------------------------------------------------ */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;        /* a(0) = 0 */
        cache_upto = 0;
    }
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    if (n <= cache_upto)
        return cache[(size_t)n];

    /* ----- ensure powers of two array ------------------------------------- */
    if (pow2_cap == 0) {
        pow2_cap = 1;
        pow2 = (seq_t *)malloc(pow2_cap * sizeof(seq_t));
        if (!pow2) abort();
        pow2[0] = (seq_t)1;          /* 2^0 = 1 */
        pow2_upto = 0;
    }
    if (pow2_cap <= (size_t)n) {
        size_t new_cap = pow2_cap ? pow2_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(pow2, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        pow2 = tmp;
        pow2_cap = new_cap;
    }
    while (pow2_upto < (idx_t)n) {
        pow2_upto++;
        pow2[pow2_upto] = pow2[pow2_upto - 1] * (seq_t)2;
    }

    /* ----- ensure Stirling array ------------------------------------------- */
    if (S_cap == 0) {
        S_cap = 1;
        S = (seq_t *)malloc(S_cap * sizeof(seq_t));
        if (!S) abort();
        S[0] = (seq_t)1;             /* S(0,0) = 1 */
        S_upto = 0;
    }
    if (S_cap <= (size_t)n) {
        size_t new_cap = S_cap ? S_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        S = tmp;
        S_cap = new_cap;
    }

    /* ----- compute a(m) for all m = S_upto+1 .. n -------------------------- */
    for (idx_t m = S_upto + 1; m <= n; ++m) {
        /* update Stirling numbers for row m (in place, descending k) */
        for (idx_t k = m; k >= 1; --k) {
            S[k] = S[k] * (seq_t)k + S[k - 1];
        }
        S[0] = (seq_t)0;  /* S(m,0) = 0 for m > 0 */

        /* compute a(m) = Σ k*2^(m-k)*S(m,k) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = S[k] * (seq_t)k;                    /* k*S(m,k)   */
            term = term * pow2[(size_t)(m - k)];            /* *2^(m-k)   */
            sum += term;
        }
        cache[(size_t)m] = sum;
    }

    S_upto = n;
    cache_upto = n;
    return cache[(size_t)n];
}
