/*
 * A253622  Centered heptagonal numbers (A069099) which are also centered
 *          pentagonal numbers (A005891).
 * https://oeis.org/A253622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(1) = 1 * a(2) = 106 * a(3) = 15016 * a(n) = 143*a(n-1) -
 *            143*a(n-2) + a(n-3) for n >= 4 * This recurrence is used to
 *            compute the sequence non‑recursive
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache the already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0; /* number of terms stored (>=3 after init) */

    if (n < 1) return 0;   /* below offset, behaviour not specified */

    /* initialise first three terms */
    if (cache_len == 0) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)106;
        cache[2] = (seq_t)15016;
        cache_len = 3;
    }

    /* compute further terms on demand */
    if (n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* indices are 1‑based, array indices 0‑based */
            seq_t a1 = cache[i - 2];     /* a(i-1) */
            seq_t a2 = cache[i - 3];     /* a(i-2) */
            seq_t a3 = cache[i - 4];     /* a(i-3) */
            seq_t val = 143 * a1 - 143 * a2 + a3;

            /* grow cache if needed */
            if ((size_t)cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 3;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
    }

    return cache[n - 1];
}
