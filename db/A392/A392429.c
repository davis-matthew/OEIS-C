/*
 * A392429  a(n) = Sum_{k=0..floor(2*n/5)} binomial(2*k,2*n-5*k).
 * https://oeis.org/A392429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊2n/5⌋} binomial(2k,2n-5k) Recurrence (used for
 *            efficient computation), initial values: a(0)=1, a(1)=0, a(2)=0,
 *            a(3)=2, a(4)=0, a(5)=1, a(6)=6 a(n) = 4*a(n-3) + a(n-5) -
 *            2*a(n-6) + a(n-7) (n ≥ 7)
 * Range:     u8 n <= 16, u16 n <= 27, u32 n <= 51, u64 n <= 98, u128 n <= 193,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Initial values for n = 0 .. 6 */
static const seq_t init_vals[7] = {1, 0, 0, 2, 0, 1, 6};

/* Static cache for memoisation */
static seq_t *cache = NULL;      /* array of computed a(i) values */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static size_t cache_len = 0;     /* number of values currently in cache */

/* Compute a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    /* Return trivial base cases directly. */
    if (n < 7u) {
        return init_vals[(size_t)n];
    }

    /* Initialise or expand the cache to fit up to index n. */
    size_t needed = (size_t)n + 1u;
    if (cache == NULL) {
        cache_cap = needed;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* out‑of‑memory – should not happen */
        for (size_t i = 0u; i < 7u; ++i) {
            cache[i] = init_vals[i];
        }
        cache_len = 7u;
    } else if (needed > cache_cap) {
        cache_cap = needed;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
    }

    /* Compute missing values up to n using the recurrence. */
    while (cache_len <= n) {
        size_t i = cache_len;
        seq_t term3 = cache[i - 3u];
        seq_t term5 = cache[i - 5u];
        seq_t term6 = cache[i - 6u];
        seq_t term7 = cache[i - 7u];

        /* a(i) = 4*a(i-3) + a(i-5) - 2*a(i-6) + a(i-7) */
        seq_t val = term3 * (seq_t)4u;
        val += term5;
        val += term7;
        val -= term6 * (seq_t)2u;

        cache[i] = val;
        ++cache_len;
    }

    return cache[n];
}
