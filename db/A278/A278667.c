/*
 * A278667  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 62", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A278667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-4) - 2*a(n-5) for n >= 11 * Initial
 *            values: * a(0)=1, a(1)=3, a(2)=3, a(3)=3, a(4)=3, * a(5)=19,
 *            a(6)=51, a(7)=35, a(8)=67, a(9)=147, a(10)=371
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values (up to n = 10) */
    static const seq_t base[11] = {
        (seq_t)1,
        (seq_t)3,
        (seq_t)3,
        (seq_t)3,
        (seq_t)3,
        (seq_t)19,
        (seq_t)51,
        (seq_t)35,
        (seq_t)67,
        (seq_t)147,
        (seq_t)371
    };

    static seq_t *cache      = NULL; /* dynamically grown cache */
    static size_t cache_cap = 0;     /* capacity of cache */
    static idx_t  cache_last = 0;    /* number of cached elements */

    /* ---- initialise cache on first call --------------------------------- */
    if (!cache) {
        /* start with size enough for the base values */
        cache_cap = 64;                     /* arbitrary small start */
        if (cache_cap < 32) cache_cap = 32; /* ensure room for growth */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* out‑of‑memory: return 0 */
        /* store base values */
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = base[i];
        cache_last = 11;                     /* indices 0..10 are cached */
    }

    /* ---- expand cache if needed ---------------------------------------- */
    if (n + 1 > (idx_t)cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* out‑of‑memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ---- If requested value is already cached --------------------------------*/
    if (n < cache_last)
        return cache[n];

    /* ---- Compute missing terms ----------------------------------------- */
    for (idx_t i = cache_last; i <= n; ++i) {
        if (i <= 10) {
            /* base values already set, nothing to do */
            continue;
        } else {
            /* recurrence: a(n) = 2*a(n-1) + a(n-4) - 2*a(n-5) */
            cache[i] = cache[i-1] * (seq_t)2 + cache[i-4] - (seq_t)2 * cache[i-5];
        }
    }
    cache_last = n + 1;
    return cache[n];
}
