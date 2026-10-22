/*
 * A370178  a(n) = floor(x*a(n-1)) for n > 0 where x = 4 + 2*sqrt(6), a(0) =
 *          1.
 * https://oeis.org/A370178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 8*a(n-3); a(0)=1, a(1)=8, a(2)=71
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/*----- C++ (arbitrary‑precision) implementation ----------------------------------*/
#ifdef __cplusplus
static std::vector<seq_t> cache;   /* static cache */

static void init_cache(idx_t n)
{
    /* Ensure the first three values are present */
    if (cache.size() < 3) {
        cache.resize(3);
        cache[0] = 1;
        cache[1] = 8;
        cache[2] = 71;
    }

    /* Grow the cache to the required size */
    if (cache.size() <= n) {
        cache.resize(n + 1);
    }
}

seq_t a(idx_t n)
{
    init_cache(n);
    if (n < 3) return cache[n];

    for (idx_t i = 3; i <= n; ++i) {
        if (cache[i] != 0) continue;               /* already computed previously */
        cache[i] = 9 * cache[i - 1] - 8 * cache[i - 3];
    }
    return cache[n];
}
#else
/*----- C (integer) implementation --------------------------------------------*/
static seq_t *cache   = NULL;
static idx_t  cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;
    idx_t new_cap = n + 1;
    seq_t *new_cache = realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) { /* allocation failure → abort */
        exit(1);
    }
    /* Initialise newly allocated entries to zero */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

static void init_cache(idx_t n)
{
    ensure_cache(n);
    if (cache[0] == 0) cache[0] = 1;
    if (cache[1] == 0) cache[1] = 8;
    if (cache[2] == 0) cache[2] = 71;
}

seq_t a(idx_t n)
{
    init_cache(n);
    if (n < 3) return cache[n];

    for (idx_t i = 3; i <= n; ++i) {
        if (cache[i] != 0) continue;               /* skip if already computed */
        cache[i] = 9 * cache[i - 1] - 8 * cache[i - 3];
    }
    return cache[n];
}
#endif
