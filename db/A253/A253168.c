/*
 * A253168  Numbers k such that the octagonal number O(k) is equal to the sum
 *          of the pentagonal numbers P(m), P(m+1), P(m+2) and P(m+3) for some
 *          m.
 * https://oeis.org/A253168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+1154*a(n-2)-1154*a(n-3)-a(n-4)+a(n-5)
   Initial values: a(1)=4, a(2)=1234, a(3)=4020, a(4)=1423650, a(5)=4638692 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* memoised terms, 0‑based (a(1) -> cache[0]) */
static idx_t cache_size = 0;          /* number of cached terms */
static size_t cache_cap = 0;          /* allocated capacity */

/* Ensure that cache holds at least n terms (n ≥ 1). */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n)
        return;

    /* Grow capacity: double each time, start at 16 if empty. */
    size_t new_cap = cache_cap ?: 16;
    while (new_cap < (size_t)n)
        new_cap *= 2;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);           /* out‑of‑memory */
    cache = tmp;
    cache_cap = new_cap;

    /* Compute new terms. */
    for (idx_t i = cache_size; i < n; ++i)
    {
        seq_t val;
        if (i == 0)          val = (seq_t)4;          /* a(1) */
        else if (i == 1)     val = (seq_t)1234;       /* a(2) */
        else if (i == 2)     val = (seq_t)4020;       /* a(3) */
        else if (i == 3)     val = (seq_t)1423650;    /* a(4) */
        else if (i == 4)     val = (seq_t)4638692;    /* a(5) */
        else
        {
            seq_t a1 = cache[i-1];   /* a(n-1)   */
            seq_t a2 = cache[i-2];   /* a(n-2)   */
            seq_t a3 = cache[i-3];   /* a(n-3)   */
            seq_t a4 = cache[i-4];   /* a(n-4)   */
            seq_t a5 = cache[i-5];   /* a(n-5)   */
            val = a1 + (seq_t)1154 * a2
                    - (seq_t)1154 * a3
                    - a4 + a5;
        }
        cache[i] = val;
    }
    cache_size = n;
}

/* Return the nth term of A253168 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined behavior; harmless */
    ensure_cache(n);
    return cache[n-1];            /* cache is 0‑based */
}
