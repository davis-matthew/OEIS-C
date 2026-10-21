/*
 * A279971  Number of n X 2 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly two elements, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 30*a(n-2) + 45*a(n-3) - 30*a(n-4) + 9*a(n-5) -
 *            a(n-6) with initial terms: a(1)=1, a(2)=3, a(3)=9, a(4)=31,
 *            a(5)=108, a(6)=366. This implementation uses memoisation of the
 *            linear recurrence
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* static cache: 1‑based indexing, cache[0] unused */
static seq_t *cache     = NULL;
static size_t cache_cap   = 0;
static idx_t  cache_max   = 0;

/* Ensure that values up to n are computed and stored in cache. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;

    size_t new_cap = (n > 6 ? (size_t)(n + 1) : 7); /* at least space for 6 init terms */

    if (!cache) {
        cache = seq_alloc(new_cap);
        cache_cap = new_cap;
    } else if (new_cap > cache_cap) {
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* Initialise the first six values once. */
    if (cache_max == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)31;
        cache[5] = (seq_t)108;
        cache[6] = (seq_t)366;
    }

    /* Compute missing terms from cache_max+1 up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 6) {
            /* already initialised */
            continue;
        }
        seq_t pos = (seq_t)9   * cache[i - 1] +
                    (seq_t)45  * cache[i - 3] +
                    (seq_t)9   * cache[i - 5];
        seq_t neg = (seq_t)30  * cache[i - 2] +
                    (seq_t)30  * cache[i - 4] +
                    cache[i - 6];
        cache[i] = pos - neg;
    }
    cache_max = n;
}

/* External function required by the suite. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* below offset – undefined, return 0 */
    ensure_cache(n);
    return cache[n];
}
