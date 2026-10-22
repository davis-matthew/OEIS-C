/*
 * A393384  a(0) = 1; a(n) = a(n-1) + 2 * a(floor(n/3)).
 * https://oeis.org/A393384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = a(n-1) + 2 * a(floor(n/3)) for n≥1
 * Range:     u8 n <= 14, u16 n <= 84, u32 n <= 878, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A393384 */

#ifdef __cplusplus
/* C++ implementation – uses std::vector for dynamic memoisation */
#include <vector>

static std::vector<seq_t> cache;
static idx_t last_computed = 0;

/* Compute a(n) using memoisation */
seq_t a(idx_t n)
{
    if (cache.empty()) {
        cache.resize(1);
        cache[0] = (seq_t)1;
        last_computed = 0;
    }

    if (n <= last_computed)
        return cache[(size_t)n];

    cache.resize((size_t)(n + 1));
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[(size_t)i] = cache[(size_t)(i - 1)] + (seq_t)2 * cache[(size_t)(i / 3)];
    }
    last_computed = n;
    return cache[(size_t)n];
}

#else
/* C implementation – uses malloc/realloc for an integral seq_t */
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation table */
static idx_t cache_size = 0;     /* number of allocated slots */
static idx_t last_computed = 0;  /* highest index whose value is known */

/* Ensure the cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;

    idx_t new_size = cache_size ? (cache_size << 1) : 1;
    while (new_size <= n)
        new_size <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache && new_size > 0)
        abort();

    for (idx_t i = cache_size; i < new_size; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_size = new_size;
}

/* Compute a(n) using memoisation */
seq_t a(idx_t n)
{
    ensure_cache(n);

    if (cache[0] == 0)
        cache[0] = (seq_t)1;

    if (n == 0)
        return cache[0];

    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i)
            cache[i] = cache[i - 1] + (seq_t)2 * cache[i / 3];
        last_computed = n;
    }

    return cache[n];
}
#endif
