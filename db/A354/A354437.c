/*
 * A354437  a(n) = n! * Sum_{k=0..n} (-k)^(n-k)/k!.
 * https://oeis.org/A354437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..n} (-k)^(n-k)/k!
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes values on demand and caches all intermediate results. */
#include <stdlib.h>

static void ensure_seq_cap(seq_t **arr, size_t *cap, size_t needed) {
    if (needed <= *cap) return;
    size_t new_cap = (*cap) ? *cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < *cap; ++i) tmp[i] = (*arr)[i];
    delete[] *arr;
    *arr = tmp;
#else
    *arr = realloc(*arr, new_cap * sizeof(seq_t));
#endif
    *cap = new_cap;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static seq_t *w = NULL;
    static size_t w_cap = 0;
    static idx_t current = 0;           /* last computed index */

    if (cache == NULL) {               /* first call */
        cache_cap = 1;
        w_cap = 1;
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
        w = new seq_t[w_cap];
#else
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        w = (seq_t *)malloc(w_cap * sizeof(seq_t));
#endif
        cache[0] = 1;
        w[0] = 1;
        current = 0;
    }

    if (n > current) {
        size_t need = (size_t)(n + 1);
        ensure_seq_cap(&cache, &cache_cap, need);
        ensure_seq_cap(&w, &w_cap, need);

        for (idx_t next = current + 1; next <= n; ++next) {
            seq_t U = 0;
            for (idx_t k = 0; k <= current; ++k) {
                U += (seq_t)k * w[k];
            }
            seq_t a_next = -(seq_t)next * U + 1;
            cache[next] = a_next;

            seq_t factor = (seq_t)next;
            for (idx_t k = 0; k <= current; ++k) {
                w[k] = factor * (-(seq_t)k) * w[k];
            }
            w[next] = 1;
            current = next;
        }
    }
    return cache[n];
}
