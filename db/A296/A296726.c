/*
 * A296726  Expansion of e.g.f. arcsin(x)/(1 - x).
 * https://oeis.org/A296726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (n-2)^2*a(n-2) - (n-2)^3*a(n-3) with initial
 *            values a(0)=0, a(1)=1, a(2)=2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute term a(n) for n >= 0.
   This function memoises values in a static table so that
   calls may come in any order and each n is computed only once. */

#include <stdlib.h> /* malloc, realloc */
#if defined(__cplusplus)
#include <vector>
#endif

/* Static cache of computed terms and its current size. */
#if defined(__cplusplus)
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
#endif
static idx_t cache_size = 0;

/* Ensure that the cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

#if defined(__cplusplus)
    cache.resize((size_t)(n + 1));
#else
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    cache = new_cache;
#endif

    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[i] = 0;
            continue;
        }
        if (i == 1)
        {
            cache[i] = 1;
            continue;
        }
        if (i == 2)
        {
            cache[i] = 2;
            continue;
        }

        /* For i >= 3: use the recurrence relation. */
        seq_t a_nm1 = cache[i - 1];
        seq_t a_nm2 = cache[i - 2];
        seq_t a_nm3 = cache[i - 3];

        seq_t term1 = (seq_t)i * a_nm1;

        seq_t nm2 = (seq_t)(i - 2);
        seq_t nm2_sq = nm2 * nm2;
        seq_t term2 = nm2_sq * a_nm2;

        seq_t nm2_cube = nm2_sq * nm2;
        seq_t term3 = nm2_cube * a_nm3;

        cache[i] = term1 + term2 - term3;
    }

    cache_size = n + 1;
}

/* Public function the tests call. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
