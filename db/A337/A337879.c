/*
 * A337879  a(n) is the length of the n-th line segment to draw the squares of
 *          the Fibonacci spiral without lifting the pencil, including
 *          superpositions.
 * https://oeis.org/A337879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-6), with a(1)=1, a(2)=1, a(3)=1, a(4)=2,
 *            a(5)=1, a(6)=2, (n > 6)
 * Range:     u8 n <= 36, u16 n <= 69, u32 n <= 138, u64 n <= 276,
 *            u128 n <= 555, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Static cache for already computed terms */
#ifdef __cplusplus
static std::vector<seq_t> cache_vec;
static idx_t cache_last_index = 0;   /* last index for which a value is computed  */
static void ensure_cache_idx(idx_t n)
{
    if (cache_vec.size() <= n) cache_vec.resize(n + 1);
}
#else
static seq_t *cache_arr = NULL;
static idx_t cache_cap    = 0;          /* current capacity of cache_arr */
static idx_t cache_last_index = 0;
static void ensure_cache_idx(idx_t n)
{
    if (cache_cap <= n)
    {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache_arr, new_cap * sizeof(seq_t));
        cache_arr = tmp;
        cache_cap = new_cap;
    }
}
#endif

static void init_base_terms(void)
{
#ifdef __cplusplus
    ensure_cache_idx(6);
    cache_vec[1] = (seq_t)1;
    cache_vec[2] = (seq_t)1;
    cache_vec[3] = (seq_t)1;
    cache_vec[4] = (seq_t)2;
    cache_vec[5] = (seq_t)1;
    cache_vec[6] = (seq_t)2;
    cache_last_index = 6;
#else
    ensure_cache_idx(6);
    cache_arr[1] = (seq_t)1;
    cache_arr[2] = (seq_t)1;
    cache_arr[3] = (seq_t)1;
    cache_arr[4] = (seq_t)2;
    cache_arr[5] = (seq_t)1;
    cache_arr[6] = (seq_t)2;
    cache_last_index = 6;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used for offset 1 but safe guard */

    /* Ensure the cache can hold term n */
    ensure_cache_idx(n);

    /* Initialize base terms if not already done */
    if (cache_last_index < 6)
        init_base_terms();

    if (n <= cache_last_index)
    {
#ifdef __cplusplus
        return cache_vec[n];
#else
        return cache_arr[n];
#endif
    }

    /* Compute missing terms just once per index */
#ifdef __cplusplus
    for (idx_t i = cache_last_index + 1; i <= n; ++i)
        cache_vec[i] = cache_vec[i - 3] + cache_vec[i - 6];
    cache_last_index = n;
    return cache_vec[n];
#else
    for (idx_t i = cache_last_index + 1; i <= n; ++i)
        cache_arr[i] = cache_arr[i - 3] + cache_arr[i - 6];
    cache_last_index = n;
    return cache_arr[n];
#endif
}
