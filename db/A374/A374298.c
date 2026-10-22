/*
 * A374298  Number of growing self-avoiding walks with displacement n on a
 *          half-infinite strip of height 3 with a trapped endpoint.
 * https://oeis.org/A374298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence a(n) = 5*a(n-1) - 4*a(n-2) - 5*a(n-3) - a(n-4), with
 *            a(1)=2, a(2)=10, a(3)=40, a(4)=148 (offset 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* 1‑based cache, cache[0] unused    */
static size_t cache_cap = 0;    /* largest index stored in cache     */

/* Ensure that cache contains values up to index n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;                 /* already ready */

    /* Allocate (or reallocate) space for indices 1..n */
    size_t new_cap = (size_t)n + 1;             /* +1 because we ignore index 0 */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                           /* out of memory – caller must handle */
    cache = tmp;

    /* Base values (indices 1..5) */
    static const seq_t base[6] = {0, 2, 10, 40, 148, 526};
    size_t start = cache_cap + 1;               /* first new index */

    if (start <= 5) {                           /* fill any missing base entries */
        for (size_t i = start; i <= 5 && i <= (size_t)n; ++i)
            cache[i] = base[i];
        start = 6;
    }

    /* Recurrence for n >= 6 */
    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t v = 5 * cache[i-1];
        v -= 4 * cache[i-2];
        v -= 5 * cache[i-3];
        v -=   cache[i-4];
        cache[i] = v;
    }

    cache_cap = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for offset 1, but harmless      */
    ensure_cache(n);
    return cache[(size_t)n];
}
