/*
 * A252530  Number of (n+2) X (6+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 2 3 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A252530 – Number of (n+2)×(6+2) arrays with the given constraints.   */
/* Recurrence: a(n) = 12*a(n-3) – 32*a(n-6)  for n > 8                   */
/* Initial values (a(1)…a(8)): 932, 1000, 1760, 2440, 7184, 13184, 18656, 54464. */

#include <stdlib.h>
#include <stddef.h>

/* Static cache – indices 1…last_computed contain valid values. */
static seq_t *cache = NULL;
static idx_t  cache_size   = 0;   /* allocated capacity (bytes of indices) */
static idx_t  last_computed = 0;  /* highest index that has been filled */

/* Initialise the cache with the eight known terms. */
static void init_cache(void)
{
    /* Allocate room for indices 0…32 (enough for the first few terms). */
    cache_size = 32;
    cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
    if (!cache) abort();

    cache[1] = (seq_t)932;
    cache[2] = (seq_t)1000;
    cache[3] = (seq_t)1760;
    cache[4] = (seq_t)2440;
    cache[5] = (seq_t)7184;
    cache[6] = (seq_t)13184;
    cache[7] = (seq_t)18656;
    cache[8] = (seq_t)54464;

    last_computed = 8;
}

/* Ensure the cache holds values up to index n (inclusive). */
static void ensure_cache_up_to(idx_t n)
{
    if (n == 0) return;                /* nothing to do for n<1 */

    if (cache == NULL) init_cache();

    if (n <= last_computed) return;    /* already have what we need */

    /* Grow the array if necessary.  Careful about overflow when doubling. */
    if (cache_size <= n) {
        idx_t new_size = cache_size;
        while (new_size <= n) {
            if (new_size > (idx_t)(SIZE_MAX >> 1)) {
                new_size = n + 1;      /* avoid overflow */
                break;
            }
            new_size <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* i > 8 ensures i-3 and i-6 are already computed; use cast to seq_t. */
        cache[(size_t)i] = (seq_t)12 * cache[(size_t)(i - 3)]
                         - (seq_t)32 * cache[(size_t)(i - 6)];
    }

    last_computed = n;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* for n<1 behavior is irrelevant */
    ensure_cache_up_to(n);
    return cache[(size_t)n];
}
