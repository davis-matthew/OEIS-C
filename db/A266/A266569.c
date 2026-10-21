/*
 * A266569  a(1) = 1; thereafter a(2k) = 4k + a(k); a(2k+1) = k + a(4k+4).
 * https://oeis.org/A266569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(2k) = 4k + a(k); a(2k+1) = k + a(4k+4)
 * Range:     u8 n <= 20, u16 n <= 5120, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

/* static cache for memoisation */
static seq_t *cache = NULL;
static bool *computed = NULL;
static idx_t cache_cap = 0;

/* enlarge cache if needed (indices 0..n inclusive) */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap <= n) new_cap <<= 1;

    /* allocate new cache array using the helper that works with
       arbitrary‑precision seq_t types */
    seq_t *new_cache = alloc_cache(new_cap);
    /* allocate flag array (bool is trivial) */
    bool *new_flag  = (bool *)malloc(new_cap * sizeof(bool));
    if (!new_cache || !new_flag) abort();

    /* copy old data */
    if (cache) {
        for (idx_t i = 0; i < cache_cap; ++i) {
            new_cache[i] = cache[i];
            new_flag[i]  = computed[i];
        }
    }

    /* initialise new entries */
    for (idx_t i = cache_cap; i < new_cap; ++i) {
        new_cache[i] = (seq_t)0;
        new_flag[i]  = false;
    }

    /* free old arrays */
    if (cache) free_cache(cache);
    free(computed);

    cache = new_cache;
    computed = new_flag;
    cache_cap = new_cap;
}

/* recursive computation with memoisation */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* unused, defined for safety */

    ensure_cache(n);
    if (computed[n]) return cache[n];
    if (n == 1) {                         /* base case */
        cache[1] = (seq_t)1;
        computed[1] = true;
        return cache[1];
    }

    seq_t res;
    if ((n & 1) == 0) {                   /* even n = 2k */
        idx_t k = n >> 1;
        ensure_cache(k);
        res = ((seq_t)4 * (seq_t)k) + a(k);
    } else {                              /* odd n = 2k+1 */
        idx_t k = (n - 1) >> 1;           /* k = (n-1)/2 */
        idx_t m = 4 * k + 4;              /* m = 4k+4 = 2(n+1) */
        ensure_cache(m);
        res = ((seq_t)k) + a(m);
    }

    cache[n] = res;
    computed[n] = true;
    return res;
}
