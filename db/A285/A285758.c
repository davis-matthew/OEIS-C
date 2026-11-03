/*
 * A285758  A slow relative of Hofstadter's Q sequence.
 * https://oeis.org/A285758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 493, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9761 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-a(n-2)) + a(n-a(n-8)),
   a(1)=1, a(n)=2 for 2≤n≤8, a(9)=3. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Cache of already–computed values (1‑based index). */
    static seq_t *cache   = NULL;  /* array of seq_t */
    static idx_t  cache_cap = 0;   /* allocated size */
    static idx_t  cache_len = 0;   /* highest n computed so far */

    if (n == 0)          /* undefined for n = 0 */
        return 0;

    /* Grow the cache if necessary. */
    if (cache_cap < n) {
        idx_t new_cap = n;                 /* simple linear growth */
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (tmp == NULL)           /* allocation failed – abort */
            return 0;
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = 1;
        } else if (i <= 8) {
            cache[i - 1] = 2;
        } else if (i == 9) {
            cache[8] = 3;
        } else {
            /* a(i) = a(i-a(i‑2)) + a(i-a(i‑8)) */
            seq_t a_i_2 = cache[i - 3];   /* a(i-2) */
            seq_t a_i_8 = cache[i - 9];   /* a(i-8) */

            idx_t idx1 = (idx_t)(i - a_i_2);
            idx_t idx2 = (idx_t)(i - a_i_8);

            /* The indices are ≥ 1; array is 0‑based. */
            cache[i - 1] = cache[idx1 - 1] + cache[idx2 - 1];
        }
    }
    cache_len = n;
    return cache[n - 1];
}
