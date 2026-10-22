/*
 * A343299  a(n) = n + A000120(a(n-1)) - a(n-1), with n > 1, a(1) = 1, where
 *          A000120(x) is the binary weight of x.
 * https://oeis.org/A343299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, look, nonn
 * Formula:   a(n) = n + A000120(a(n-1)) - a(n-1), with a(1) = 1 (A000120 =
 *            binary weight). This implements the required recurrence
 * Range:     u8 n <= 502, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <vector>
#include <cstdlib>
#else
#include <stdlib.h>
#endif

/* ---- Helper: binary weight (popcount) --------------------------------- */
static seq_t popcount_seq(seq_t x)
{
    seq_t cnt = 0;
    while (x != 0)
    {
        cnt += x & 1;
        x >>= 1;
    }
    return cnt;
}

/* ---- Cache for computed terms ----------------------------------------- */
#ifdef __cplusplus
/* Use std::vector for C++ builds; the constructor will
   value‑initialize each element (zero for integral types,
   zero for many big integer classes). */
static std::vector<seq_t> cache_vec(1, seq_t());   /* index 0 unused */

/* Ensure that cache_vec contains indices up to need (inclusive). */
static void ensure_cap(idx_t need)
{
    if (need < cache_vec.size())
        return;
    cache_vec.resize(need + 1);        /* new elements are zeroed */
}
#else
/* For C builds use malloc/realloc with plain POD seq_t. */
static seq_t *cache = NULL;   /* cache[0] unused */
static idx_t  cache_cap = 0;  /* current highest index stored */

/* Ensure that cache contains indices up to need (inclusive). */
static void ensure_cap(idx_t need)
{
    if (need <= cache_cap)
        return;
    seq_t *new_cache = (seq_t*)realloc(cache, (need + 1) * sizeof(seq_t));
    if (!new_cache)
        exit(1);                     /* allocation failure – abort */
    cache = new_cache;
    /* initialise new slots to zero (uncomputed) */
    for (idx_t i = cache_cap + 1; i <= need; ++i)
        cache[i] = 0;
    cache_cap = need;
}
#endif

/* ---- Main sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for n<1, guard anyway */
        return 0;

    /* Expand cache so that index n is valid */
#ifdef __cplusplus
    ensure_cap(n);
    if (cache_vec[1] == 0)
        cache_vec[1] = 1;           /* a(1) = 1 */
    for (idx_t i = 2; i <= n; ++i)
    {
        if (cache_vec[i] != 0)     /* already computed */
            continue;
        seq_t prev = cache_vec[i - 1];
        seq_t pop  = popcount_seq(prev);
        cache_vec[i] = (seq_t)i + pop - prev;
    }
    return cache_vec[n];
#else
    ensure_cap(n);
    if (cache[1] == 0)
        cache[1] = 1;           /* a(1) = 1 */
    for (idx_t i = 2; i <= n; ++i)
    {
        if (cache[i] != 0)       /* already computed */
            continue;
        seq_t prev = cache[i - 1];
        seq_t pop  = popcount_seq(prev);
        cache[i] = (seq_t)i + pop - prev;
    }
    return cache[n];
#endif
}
