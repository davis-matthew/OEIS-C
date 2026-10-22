/*
 * A387342  Expansion of 1/(1 - 6*x + x^2)^(7/2).
 * https://oeis.org/A387342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 21; Recurrence: for n > 1, n*a(n) =
 *            3*(2*n+5)*a(n-1) - (n+5)*a(n-2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses memoisation.  */

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 21;

    if (cache.empty())
    {
        cache.reserve(n + 1);
        cache.push_back(1);   // a(0)
        cache.push_back(21);  // a(1)
    }

    for (size_t i = cache.size(); i <= (size_t)n; ++i)
    {
        seq_t term1 = (seq_t)3 * ( ((seq_t)2 * (seq_t)i + 5) * cache[i - 1] );
        seq_t term2 = ((seq_t)(i + 5)) * cache[i - 2];
        seq_t val = (term1 - term2) / (seq_t)i;
        cache.push_back(val);
    }
    return cache[(size_t)n];
}
#else /* C */
#include <stdlib.h>
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of valid entries in cache */
static size_t cache_cap = 0;   /* allocated capacity */

static void ensure_capacity(size_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();  /* allocation failed */
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 21;

    /* make sure we can index up to n */
    ensure_capacity((size_t)n + 1);

    /* initialise base values if cache is empty */
    if (cache_len == 0)
    {
        cache[0] = 1;
        cache[1] = 21;
        cache_len = 2;
    }

    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        /* Use 128‑bit intermediate for safety on 64‑bit builds */
        unsigned __int128 t1 = (unsigned __int128)3 *
            (((unsigned __int128)2 * (unsigned __int128)i + 5) *
             (unsigned __int128)cache[i - 1]);
        unsigned __int128 t2 = (unsigned __int128)(i + 5) *
            (unsigned __int128)cache[i - 2];
        cache[i] = (seq_t)((t1 - t2) / (unsigned __int128)i);
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
#endif
