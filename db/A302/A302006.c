/*
 * A302006  Number of nX4 0..1 arrays with every element equal to 0, 1, 2 or 3
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A302006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) + 18*a(n-2) + a(n-3) - 4*a(n-4) (n≥5) a(1)=8,
 *            a(2)=128, a(3)=1808, a(4)=25808
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

/* ------------------------------------------------------------------ */
/* Cache handling (no global state other than the cache itself)      */
/* ------------------------------------------------------------------ */
#ifndef __cplusplus
static seq_t *cache       = NULL;   /* cache[0] unused, cache[1] holds a(1)  */
static size_t cache_cap  = 0;       /* allocated capacity (elements)       */

static void ensure_cache_size(idx_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = n + 1;
    if (new_cap < 8) new_cap = 8;
    while (new_cap <= (size_t)n) new_cap <<= 1;          /* power‑of‑two */
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                                   /* OOM – caller gets 0 */
    /* initialise the newly allocated part to zero */
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}
#else
static std::vector<seq_t> cache;   /* cache[0] unused, cache[1] holds a(1) */

static void ensure_cache_size(idx_t n)
{
    size_t need = static_cast<size_t>(n) + 1;
    if (cache.size() < need) cache.resize(need);
}
#endif

/* ------------------------------------------------------------------ */
/* Public function                                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1 – out‑of‑range returns 0   */
        return 0;

    /* make sure storage exists */
    ensure_cache_size(n);

    /* return a cached value if available */
    if (cache[(size_t)n]) return cache[(size_t)n];

    /* initialise the first four terms (index 1..4) */
    static const seq_t init[5] = { 0, 8, 128, 1808, 25808 };

    /* compute all terms up to n, filling gaps if any */
    for (size_t i = 1; i <= (size_t)n; ++i)
    {
        if (cache[i]) continue;          /* already computed */

        seq_t val;
        if (i <= 4)
            val = init[i];
        else
            val = 13 * cache[i - 1] + 18 * cache[i - 2]
                    + cache[i - 3] - 4 * cache[i - 4];

        cache[i] = val;
    }

    return cache[(size_t)n];
}
