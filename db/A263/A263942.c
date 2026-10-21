/*
 * A263942  Positive integers m such that (m+4)^3 - m^3 is a square.
 * https://oeis.org/A263942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1)-5*a(n-2)+a(n-3) with a(1) = 6, a(2) = 28, a(3) =
 *            110 (OEIS A263942)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

static void
ensure_cache(size_t needed, seq_t **p_cache, size_t *p_cap)
{
    if (needed <= *p_cap)
        return;
    size_t new_cap = (*p_cap) ? (*p_cap) * 2 : 4;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(*p_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failed */
    *p_cache = tmp;
    *p_cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1 */
    if (n == 0)
        return (seq_t)0;

    /* Static cache for memoised values */
    static seq_t *cache      = NULL;
    static size_t cache_sz   = 0;    /* number of terms stored */
    static size_t cache_cap  = 0;    /* allocated capacity */

    /* Initialise first three terms once */
    if (cache_sz == 0) {
        ensure_cache(3, &cache, &cache_cap);
        cache[0] = (seq_t)6;
        cache[1] = (seq_t)28;
        cache[2] = (seq_t)110;
        cache_sz = 3;
    }

    /* Return cached value if already computed */
    if ((size_t)(n - 1) < cache_sz)
        return cache[n - 1];

    /* Ensure cache can hold all terms up to n */
    size_t target = (size_t)n;
    ensure_cache(target, &cache, &cache_cap);

    /* Compute missing terms using the recurrence */
    while (cache_sz < target) {
        seq_t a1 = cache[cache_sz - 1];
        seq_t a2 = cache[cache_sz - 2];
        seq_t a3 = cache[cache_sz - 3];
        seq_t next = (seq_t)5 * a1 - (seq_t)5 * a2 + a3;
        cache[cache_sz] = next;
        ++cache_sz;
    }

    return cache[n - 1];
}
