/*
 * A380309  Expansion of e.g.f. exp( 1 - 1/(1-3*x)^(1/3) ).
 * https://oeis.org/A380309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 9, i64 n <= 16, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A380309
 * Formula: a(n) = -Σ_{k=1..n} B(k) * C(n‑1, k‑1) * a(n-k)
 * where B(k) = ∏_{j=0..k‑2} (3j+1)   (B(1)=1)
 *
 * The implementation uses dynamic arrays that grow as needed.
 * It is valid C11 and C++17; in C++ placement new is used to
 * construct elements of a class type seq_t.
 */

#include <stdlib.h>
#ifdef __cplusplus
# include <new>
#endif

seq_t a(idx_t n)
{
    /* Static state ------------------------------------------------------- */
    static seq_t *a_cache = NULL;    /* a_cache[i] = a(i)            */
    static size_t   a_cap   = 0;      /* current capacity of a_cache   */
    static idx_t    a_max   = 0;      /* largest n computed so far    */

    static seq_t *b_cache = NULL;    /* b_cache[k] = B(k)            */
    static size_t   b_cap   = 0;      /* capacity of b_cache          */
    static idx_t    b_max   = 0;      /* largest k computed           */

    /* Init a_cache ------------------------------------------------------- */
    if (a_cache == NULL) {
        a_cap = 1;
        a_cache = (seq_t*)malloc(a_cap * sizeof(seq_t));
        if (!a_cache) return 0;
#ifdef __cplusplus
        new(&a_cache[0]) seq_t();
#endif
        a_cache[0] = (seq_t)1;       /* a(0) = 1 */
        a_max = 0;
    }

    /* Init b_cache ------------------------------------------------------- */
    if (b_cache == NULL) {
        b_cap = 2;                   /* indices 0 and 1 allocated */
        b_cache = (seq_t*)malloc(b_cap * sizeof(seq_t));
        if (!b_cache) return 0;
#ifdef __cplusplus
        new(&b_cache[0]) seq_t();
        new(&b_cache[1]) seq_t();
#endif
        b_cache[0] = (seq_t)1;       /* unused, but set to 1 */
        b_cache[1] = (seq_t)1;       /* B(1) = 1 */
        b_max = 1;
    }

    /* Grow a_cache if necessary ------------------------------------------ */
    if ((size_t)(n + 1) > a_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        for (size_t i = a_cap; i < new_cap; ++i) {
#ifdef __cplusplus
            new(&tmp[i]) seq_t();
#endif
        }
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* Grow b_cache if necessary ------------------------------------------ */
    if ((size_t)(n + 1) > b_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(b_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        for (size_t i = b_cap; i < new_cap; ++i) {
#ifdef __cplusplus
            new(&tmp[i]) seq_t();
#endif
        }
        b_cache = tmp;
        b_cap   = new_cap;
    }

    /* Compute missing B(k) values --------------------------------------- */
    for (idx_t k = b_max + 1; k <= n; ++k) {
        seq_t factor = (seq_t)(3 * (k - 1) + 1);   /* 3*(k-1)+1 */
        b_cache[k] = b_cache[k - 1] * factor;
    }
    if (n > (idx_t)b_max) b_max = n;

    /* Compute a(n) if not already done ----------------------------------- */
    for (idx_t m = a_max + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;   /* C(m-1,0) */

        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = b_cache[k] * binom * a_cache[m - k];
            sum += term;

            /* Update binomial for next k: C(m-1,k) = binom * (m-k)/k */
            if (k < m) {
                binom = binom * (seq_t)(m - k) / (seq_t)k;
            }
        }
        a_cache[m] = -sum;
    }
    if (n > a_max) a_max = n;

    return a_cache[n];
}
