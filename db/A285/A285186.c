/*
 * A285186  Expansion of (x+2*x^2) / (1-2*x-2*x^2+2*x^3).
 * https://oeis.org/A285186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 2*a(n-3) for n > 2; a(0) = 0, a(1)
 *            = 1, a(2) = 4
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

static seq_t *cache_c = NULL;   /* dynamic array for C builds          */
static size_t cache_size_c = 0; /* number of valid entries in cache_c   */

#ifdef __cplusplus
static std::vector<seq_t> cache_cpp; /* dynamic vector for C++ builds       */
#endif

/* Grow the cache so that cache[n] is defined for the requested n.   */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;

#ifdef __cplusplus
    /* Make sure the first three values are in place. */
    if (cache_cpp.size() < 3)
    {
        cache_cpp.resize(3);
        cache_cpp[0] = (seq_t)0;
        cache_cpp[1] = (seq_t)1;
        cache_cpp[2] = (seq_t)4;
    }

    /* Compute new entries if required. */
    size_t old = cache_cpp.size();
    if (old < 3) old = 3;          /* safety for very first call      */
    if (old < need)
    {
        cache_cpp.resize(need);
        for (size_t i = old; i < need; ++i)
        {
            cache_cpp[i] = 2 * cache_cpp[i-1] + 2 * cache_cpp[i-2] - 2 * cache_cpp[i-3];
        }
    }
#else
    /* First time: allocate space for the three seed values. */
    if (cache_size_c < 3)
    {
        seq_t *tmp = realloc(cache_c, 3 * sizeof(seq_t));
        if (!tmp) abort();
        cache_c = tmp;
        cache_c[0] = (seq_t)0;
        cache_c[1] = (seq_t)1;
        cache_c[2] = (seq_t)4;
        cache_size_c = 3;
    }

    /* Grow the array if necessary. */
    if (cache_size_c < need)
    {
        seq_t *tmp = realloc(cache_c, need * sizeof(seq_t));
        if (!tmp) abort();
        cache_c = tmp;
        for (size_t i = cache_size_c; i < need; ++i)
        {
            cache_c[i] = 2 * cache_c[i-1] + 2 * cache_c[i-2] - 2 * cache_c[i-3];
        }
        cache_size_c = need;
    }
#endif
}

/* Return the n-th term of A285186 (offset 0). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    ensure_cache(n);

#ifdef __cplusplus
    return cache_cpp[(size_t)n];
#else
    return cache_c[(size_t)n];
#endif
}
