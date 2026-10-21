/*
 * A252004  Numbers n such that the sum of the pentagonal numbers P(n) and
 *          P(n+1) is equal to the sum of the octagonal numbers N(m) and
 *          N(m+1) for some m.
 * https://oeis.org/A252004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A252004
   Recurrence: a(n) = 1155*a(n-1) - 1155*a(n-2) + a(n-3)
   Initial values: a(1)=328, a(2)=378896, a(3)=437246040
   Offset: 1
*/

#include <stdlib.h>
#include <stdint.h>

/* Memoisation cache (1‑based index). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;              /* allocated slots */
static size_t cache_count = 0;            /* highest n computed */

/* Ensure cache holds at least n elements. */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache) abort();                  /* allocation failed */
    cache_cap = new_cap;
}

seq_t
a(idx_t n)
{
    if (n < 1) return 0;                  /* undefined for n<1 */

    if ((size_t)n > cache_cap)
        ensure_cache(n);

    /* Already computed? */
    if ((size_t)n <= cache_count)
        return cache[n - 1];

    /* Initial values (constants fit into built‑in types). */
    const seq_t init1 = 328;
    const seq_t init2 = 378896;
    const seq_t init3 = 437246040;

    size_t i;
    for (i = cache_count + 1; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 1)
            val = init1;
        else if (i == 2)
            val = init2;
        else if (i == 3)
            val = init3;
        else {
            /* a(i) = 1155*a(i-1) - 1155*a(i-2) + a(i-3) */
            const seq_t a1 = cache[i - 2];   /* a(i-1) */
            const seq_t a2 = cache[i - 3];   /* a(i-2) */
            const seq_t a3 = cache[i - 4];   /* a(i-3) */
            val = a1 * (seq_t)1155 - a2 * (seq_t)1155 + a3;
        }
        cache[i - 1] = val;
    }
    cache_count = (size_t)n;
    return cache[n - 1];
}
