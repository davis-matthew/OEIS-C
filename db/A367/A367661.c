/*
 * A367661  G.f. A(x) satisfies A(x) = 1 / (1 - x - x * A(x^5)).
 * https://oeis.org/A367661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = a(n-1) + Σ_{k=0..⌊(n-1)/5⌋} a(k)·a(n-1-5·k)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for the OEIS sequence A367661.
   The function is memoised using a static array that grows on demand.
   It works for any seq_t and idx_t types provided by the build. */
#include <stdlib.h>

/* Ensure that the cache has room for indices up to n (inclusive). */
static void ensure_cache(idx_t n, seq_t **cache, size_t *cap)
{
    if (*cap > n) return;                       /* already enough space */
    size_t new_cap = (*cap == 0) ? 16 : (*cap * 2);
    while (new_cap <= n) new_cap *= 2;           /* grow until n+1 slots available */
    /* Reallocate and cast for C++ compatibility. */
    seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    if (!tmp) { exit(EXIT_FAILURE); }           /* out of memory */
    *cache = tmp;
    *cap   = new_cap;
}

/* Static cache: array of a(i) values, plus maintenance counters. */
static seq_t *cache   = NULL;   /* allocated buffer */
static size_t cache_cap = 0;    /* current capacity (number of slots) */
static size_t cache_size = 0;   /* number of values already computed (last index+1) */

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Handle n = 0 directly. */
    if (n == 0) return (seq_t)1;

    /* Make sure the array can hold indices up to n. */
    if (n >= cache_cap) ensure_cache(n, &cache, &cache_cap);

    /* Compute missing values up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {                     /* a(0) = 1 */
            cache[i] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        idx_t bound = (i - 1) / 5;
        for (idx_t k = 0; k <= bound; ++k)
            sum += cache[k] * cache[(i - 1) - 5 * k];
        cache[i] = cache[i - 1] + sum;
    }
    cache_size = n + 1;                    /* all values up to n are now ready */

    return cache[n];
}
