/*
 * A275178  Number of n X 3 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-1,0) or (-1,1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 57, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A275178:
 *   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - a(n-4) - a(n-5)   for n>8
 * Initial terms:
 *   a(1)=5, a(2)=16, a(3)=25, a(4)=41, a(5)=85,
 *   a(6)=181, a(7)=389, a(8)=834
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache of computed terms.
 * index 0 is unused; cache_highest is the largest n already computed. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* allocated capacity (indices 0..cache_cap-1) */
static idx_t cache_highest = 0;   /* highest stored term index */

/* Ensure the cache can hold at least indices 0..n. */
static void ensure_cache_cap(idx_t n)
{
    if (cache_cap >= (size_t)n + 1)
        return;                   /* already large enough */

    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 8;
    if (new_cap < (size_t)n + 1)
        new_cap = (size_t)n + 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache with the first eight terms. */
static void init_cache(void)
{
    if (cache_highest != 0)
        return;

    /* Ensure capacity for indices 0..8. */
    ensure_cache_cap(8);

    cache[1] = (seq_t)5;
    cache[2] = (seq_t)16;
    cache[3] = (seq_t)25;
    cache[4] = (seq_t)41;
    cache[5] = (seq_t)85;
    cache[6] = (seq_t)181;
    cache[7] = (seq_t)389;
    cache[8] = (seq_t)834;

    cache_highest = 8;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    init_cache();                 /* ensure base terms are available */

    if (n <= cache_highest)
        return cache[n];          /* already computed */

    /* Allocate sufficient space. */
    ensure_cache_cap(n);

    /* Compute missing terms up to n. */
    for (idx_t i = cache_highest + 1; i <= n; ++i) {
        /* Recurrence: a(i) = 4*a(i-1) - 6*a(i-2) + 5*a(i-3) - a(i-4) - a(i-5) */
        seq_t term1 = (seq_t)4 * cache[i - 1];
        term1 += (seq_t)5 * cache[i - 3];

        seq_t term2 = (seq_t)6 * cache[i - 2];
        term2 += cache[i - 4];
        term2 += cache[i - 5];

        cache[i] = term1 - term2;
    }

    cache_highest = n;
    return cache[n];
}
