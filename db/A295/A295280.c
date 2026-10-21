/*
 * A295280  a(n) = 1 + the number of distinct earlier terms that are coprime
 *          to n.
 * https://oeis.org/A295280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + number of distinct earlier terms that are coprime to
 *            n
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Cache for already computed a(n) values. */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms currently stored */
static size_t cache_cap   = 0;   /* allocated capacity of cache */

/* Timestamped seen array to count distinct a(k) values. */
static unsigned int *seen_time = NULL;
static size_t seen_cap   = 0;
static unsigned int seen_clock = 1;

/* Ensure cache can hold at least up to index n. */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    /* double until large enough */
    while (new_cap < (size_t)n) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* Ensure seen_time can be indexed up to max_val (inclusive). */
static void ensure_seen_capacity(idx_t max_val)
{
    size_t need = (size_t)max_val + 1;
    if (need <= seen_cap) return;
    size_t new_cap = seen_cap ? seen_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    unsigned int *new_seen = (unsigned int *)realloc(seen_time,
                                                     new_cap * sizeof(unsigned int));
    if (!new_seen) abort();
    /* initialise new entries to 0 */
    memset(new_seen + seen_cap, 0, (new_cap - seen_cap) * sizeof(unsigned int));
    seen_time = new_seen;
    seen_cap = new_cap;
}

/* Return the OEIS sequence value a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* graceful handling of index 0 */

    /* Grow the cache if necessary. */
    ensure_cache_capacity(n);

    /* If already computed, return cached value. */
    if (cache_size >= (size_t)n)
        return cache[(size_t)n - 1];

    /* Compute all missing terms from cache_size+1 up to n. */
    for (size_t m = cache_size + 1; m <= (size_t)n; ++m) {
        seq_t val = 1;            /* base 1 */

        /* Prepare seen_time for this m. */
        ++seen_clock;
        ensure_seen_capacity((idx_t)m);  /* a(k) <= k <= m */

        /* Examine all earlier terms k < m. */
        for (size_t k = 1; k < m; ++k) {
            seq_t a_prev = cache[k - 1];
            /* a_prev coprime to m? */
            if (gcd(a_prev, (seq_t)m) == 1) {
                size_t idx = (size_t)a_prev;   /* a_prev <= k <= m-1 */
                if (seen_time[idx] != seen_clock) {
                    seen_time[idx] = seen_clock;
                    ++val;
                }
            }
        }
        cache[m - 1] = val;
    }

    cache_size = (size_t)n;
    return cache[(size_t)n - 1];
}
