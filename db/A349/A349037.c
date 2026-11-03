/*
 * A349037  G.f. A(x) satisfies: A(x) = 1 / (1 - x - x^2 * A(-4*x)).
 * https://oeis.org/A349037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1, a(1) = 1 a(n) = a(n-1) + Sum_{k=0..n-2} (-4)^k * a(k)
 *            * a(n-k-2) for n ≥ 2 (i.e. the recurrence given in the OEIS
 *            entry)
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 9, i64 n <= 12, i128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* array of computed terms */
    static size_t cache_cap = 0;       /* allocated capacity */
    static idx_t  cache_len = 0;       /* number of computed terms */

    /* make sure enough space is available */
    if ((size_t)(n + 1) > cache_cap)
        ensure_capacity((size_t)(n + 1), &cache_cap, (void **)&cache, sizeof(seq_t));

    /* compute missing terms */
    if (cache_len <= n) {
        for (idx_t m = cache_len; m <= n; ++m) {
            if (m == 0) { cache[0] = 1; continue; }
            if (m == 1) { cache[1] = 1; continue; }

            seq_t sum   = 0;
            seq_t power = 1;               /* power = (-4)^k, starts with k=0 */

            for (idx_t k = 0; k <= m - 2; ++k) {
                sum += power * cache[k] * cache[m - 2 - k];
                power *= (seq_t)-4;
            }
            cache[m] = cache[m - 1] + sum;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
