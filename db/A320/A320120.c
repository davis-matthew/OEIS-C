/*
 * A320120  Expansion of Product_{k>0} 1/(Sum_{m>=0} x^(k*m^3)).
 * https://oeis.org/A320120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in * ∏_{k>0} 1/(∑_{m≥0} x^{k·m³}) *
 *            computed by successive division of power series: * a_0(x) = 1 *
 *            a_{k+1}(x) = a_k(x) / (1 + x^{k·1³} + x^{k·2³} + …) * The
 *            coefficients of each factor are obtained by truncating at degree
 *            n. * The algorithm runs in O(n²) time, easily fa...
 * Range:     i8 n <= 37, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Assume seq_t, idx_t are defined by the build.  seq_t supports +, -, *,
 *  and assignment operators.  idx_t is an unsigned integer type. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;     /* cached results up to cache_cap-1   */
    static idx_t cache_cap = 0;     /* number of stored elements (== cap) */

    /* (1) Return cached value if possible. */
    if (n < cache_cap) {
        return cache[(size_t)n];
    }

    /* (2) Compute all coefficients up to n. */
    size_t N = (size_t)n;                   /* N fits into size_t (n ≈ 10⁴)     */
    size_t len = N + 1;                     /* length of arrays                */

    /* Allocate two working arrays of size len. */
    seq_t *a_cur = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *a_next = (seq_t *)calloc(len, sizeof(seq_t));
    if (!a_cur || !a_next) {                /* Abort on allocation failure   */
        if (a_cur) free(a_cur);
        if (a_next) free(a_next);
        /* In a real program we might abort or return an error code. */
        return (seq_t)0;
    }

    /* Initial series: a_0(x) = 1 */
    a_cur[0] = (seq_t)1;          /* all other entries already zero  */

    /* Iterate over k = 1 … n  (only these factors affect degree ≤ n). */
    for (idx_t k = 1; k <= n; ++k) {
        a_next[0] = a_cur[0];      /* leading coefficient is 1           */

        /* For each degree n_i = 1 .. N */
        for (size_t n_i = 1; n_i <= N; ++n_i) {
            seq_t sum = (seq_t)0;

            /* Subtract contributions from all offsets k·m³ ≤ n_i. */
            for (uint64_t m = 1;; ++m) {
                uint64_t cube = m * m * m;          /* m³  (fits into 64‑bit)      */
                uint64_t offset = (uint64_t)k * cube;
                if (offset > n_i) break;
                sum += a_next[n_i - (size_t)offset];
            }

            a_next[n_i] = a_cur[n_i] - sum;
        }

        /* Swap the two arrays for next iteration. */
        seq_t *tmp = a_cur;
        a_cur = a_next;
        a_next = tmp;
    }

    /* (3) Store the computed values in the cache. */
    seq_t *new_cache = (seq_t *)malloc(len * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failure: clean up and return zero. */
        free(a_cur);
        free(a_next);          /* a_next is the old array after final swap   */
        return (seq_t)0;
    }
    for (size_t i = 0; i < len; ++i) {
        new_cache[i] = a_cur[i];
    }

    /* Free old cache and working array that is no longer needed. */
    if (cache) free(cache);
    free(a_cur);
    free(a_next);                 /* a_next holds the discarded array          */

    cache = new_cache;
    cache_cap = (idx_t)len;       /* new number of stored elements            */

    return cache[(size_t)n];
}
