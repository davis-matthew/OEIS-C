/*
 * A314068  Coordination sequence Gal.3.55.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-5) - a(n-6) + a(n-7) -
 *            a(n-8) with initial values a(0)=1, a(1)=5, a(2)=11, a(3)=15,
 *            a(4)=22, a(5)=27, a(6)=31, a(7)=37, a(8)=42
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#include <cstddef>
#include <cstdlib>

static std::vector<seq_t> cache;

/* Return the nth element of the coordination sequence */
seq_t a(idx_t n)
{
    /* Ensure the cache holds the first nine terms */
    if (cache.empty()) {
        cache.resize(9);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)22;
        cache[5] = (seq_t)27;
        cache[6] = (seq_t)31;
        cache[7] = (seq_t)37;
        cache[8] = (seq_t)42;
        /* The vector may have extra capacity, but size() is 9 now */
    }

    /* If the requested term is already computed, return it */
    if (n < cache.size())
        return cache[n];

    /* Compute any missing terms up to n */
    for (idx_t i = cache.size(); i <= n; ++i) {
        seq_t val = cache[i-1] - cache[i-2] + cache[i-3]
                    + cache[i-5] - cache[i-6] + cache[i-7]
                    - cache[i-8];
        cache.push_back(val);
    }

    return cache[n];
}

#else /* C implementation, seq_t is an integer type */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Ensure that the cache has room for at least `needed` elements */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < needed)
        new_cap <<= 1;                /* grow exponentially */
    seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth element of the coordination sequence */
seq_t a(idx_t n)
{
    /* Initialize cache with the first nine terms */
    if (!cache) {
        cache_cap = 16;               /* a bit more than needed */
        cache = calloc(cache_cap, sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)22;
        cache[5] = (seq_t)27;
        cache[6] = (seq_t)31;
        cache[7] = (seq_t)37;
        cache[8] = (seq_t)42;
        cache_len = 9;
    }

    /* If the requested term is already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* Compute any missing terms up to n */
    ensure_capacity(n + 1);
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = cache[i-1] - cache[i-2] + cache[i-3]
                    + cache[i-5] - cache[i-6] + cache[i-7]
                    - cache[i-8];
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
#endif
