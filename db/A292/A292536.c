/*
 * A292536  p-INVERT of the squares (A000290), where p(S) = 1 + S - 3 S^2.
 * https://oeis.org/A292536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 14*a(n-2) + 26*a(n-3) - 10*a(n-4) + 5*a(n-5) -
 *            a(n-6) * Initial values (n = 0..6): 1, 8, 48, 255, 1310, 6773,
 *            35260 * The sequence is defined by the p‑INVERT of the squares
 *            with p(S)=1+S-3S^2. * This implementation memoises the results
 *            so that calls with arbitrary * non‑increasi...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of the sequence. */
static seq_t *seq_cache = NULL;
static size_t cache_capacity = 0;   /* allocated size */
static size_t cache_size    = 0;    /* number of valid terms in seq_cache */

/* Initialise the cache with the first seven terms. */
static void init_cache(void)
{
    if (seq_cache != NULL) return;   /* already initialised */

    /* Start with a modest capacity and grow exponentially. */
    cache_capacity = 64;
    seq_cache = (seq_t*)malloc(cache_capacity * sizeof(seq_t));
    if (!seq_cache) return;   /* allocation failed; callers will crash soon */

    /* Insert the known initial terms. */
    seq_cache[0] = (seq_t)1;
    seq_cache[1] = (seq_t)8;
    seq_cache[2] = (seq_t)48;
    seq_cache[3] = (seq_t)255;
    seq_cache[4] = (seq_t)1310;
    seq_cache[5] = (seq_t)6773;
    seq_cache[6] = (seq_t)35260;
    cache_size = 7;
}

/* Ensure the cache can accommodate at least indices up to n. */
static void grow_cache(idx_t n)
{
    if (cache_size > n) return;

    /* Expand the cache exponentially until it can hold n. */
    while (cache_size <= n) {
        size_t new_capacity = cache_capacity ? cache_capacity * 2 : 64;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_capacity * sizeof(seq_t));
        if (!tmp) return;   /* allocation failed; callers will crash soon */
        seq_cache = tmp;
        cache_capacity = new_capacity;
    }
}

/* Compute a(n) using the linear recurrence. */
seq_t a(idx_t n)
{
    init_cache();
    if (n < cache_size) return seq_cache[n];

    grow_cache(n);

    /* Compute terms from cache_size up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Use the recurrence: a(i) = 7*a(i-1) - 14*a(i-2) + 26*a(i-3)
         *                - 10*a(i-4) + 5*a(i-5) - a(i-6) */
        seq_t term = 7 * seq_cache[i-1];
        term -= 14 * seq_cache[i-2];
        term += 26 * seq_cache[i-3];
        term -= 10 * seq_cache[i-4];
        term += 5 * seq_cache[i-5];
        term -= seq_cache[i-6];

        seq_cache[i] = term;
    }

    cache_size = n + 1;  /* all computed values are now valid */
    return seq_cache[n];
}
