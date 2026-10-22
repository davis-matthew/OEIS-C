/*
 * A330615  a(0) = 1; a(1) = 1; a(n) = a(a(n - 1) mod n) + a(a(n - 2) mod n).
 * https://oeis.org/A330615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 1 a(n) = a(a(n‑1) mod n) + a(a(n‑2) mod n) for n
 *            ≥ 2
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* compute the nth term of OEIS sequence A330615 */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* array of values a(0)…a(k) */
    static idx_t cached_size = 0; /* number of valid entries in cache */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 2);
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cached_size = 2;
    }

    /* grow cache if n exceeds current size */
    if (n >= cached_size) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;

        /* compute missing terms from cached_size up to n */
        for (idx_t i = cached_size; i <= n; ++i) {
            seq_t a_nm1 = cache[i-1];
            seq_t a_nm2 = cache[i-2];

            /* indices for recursive calls */
            idx_t idx1 = (idx_t)(a_nm1 % (seq_t)i);
            idx_t idx2 = (idx_t)(a_nm2 % (seq_t)i);

            cache[i] = cache[idx1] + cache[idx2];
        }
        cached_size = new_cap;
    }

    return cache[n];
}
