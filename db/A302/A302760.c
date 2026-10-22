/*
 * A302760  Number of total dominating sets in the n-antiprism graph.
 * https://oeis.org/A302760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + 6*a(n-3) - 3*a(n-4) + a(n-7) for n >
 *            7, with base values: a(1)=3, a(2)=11, a(3)=54, a(4)=179,
 *            a(5)=648, a(6)=2414, a(7)=8809
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* static cache (1‑based indexing, cache[0] unused) */
static seq_t *cache    = NULL;   /* pointer to array of computed terms */
static size_t cache_cap  = 0;    /* how many elements are allocated */
static size_t last_comp  = 0;    /* largest index whose value is already stored */

seq_t a(idx_t n)
{
    if (n <= 0) return 0;                /* safe guard – sequence defined for n≥1 */

    /* 1.  Ensure the cache can hold indices up to n (n+1 elements). */
    size_t needed = (size_t)n + 1;        /* include index 0 */
    if (cache_cap < needed)
    {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failed */
        cache = tmp;
        cache_cap = needed;
    }

    /* 2.  Initialise the 7 known base terms once. */
    if (last_comp < 7)
    {
        /* Ensure the array can store indices 0 … 7. */
        if (cache_cap < 8)
        {
            seq_t *tmp = (seq_t *)realloc(cache, 8 * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = 8;
        }
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)54;
        cache[4] = (seq_t)179;
        cache[5] = (seq_t)648;
        cache[6] = (seq_t)2414;
        cache[7] = (seq_t)8809;
        last_comp = 7;
    }

    /* 3.  Compute any missing terms up to n. */
    for (size_t k = last_comp + 1; k <= (size_t)n; ++k)
    {
        seq_t term = 3 * cache[k-1] +       /* 3*a(k-1)   */
                     cache[k-2] +            /* + a(k-2)     */
                     6 * cache[k-3] +       /* + 6*a(k-3)   */
                     cache[k-7] -           /* + a(k-7)     */
                    3 * cache[k-4];         /* - 3*a(k-4)   */
        cache[k] = term;
    }
    last_comp = (size_t)n;
    return cache[(size_t)n];
}
