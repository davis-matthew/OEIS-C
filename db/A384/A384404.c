/*
 * A384404  Consecutive states of the linear congruential pseudo-random number
 *          generator for Turbo Pascal when started at 1.
 * https://oeis.org/A384404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1 a(n) = (134775813 * a(n-1) + 1) mod 2^32 for n > 1
 *            Recurrence defined by the Turbo Pascal LCG
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for memoisation; 1‑based indices, cache[0] unused   */
static seq_t *cache = NULL;
static idx_t cache_max = 0;
/* 2^32 – 1, cast to seq_t keeps only the low 32 bits                 */
static const seq_t MODMASK = (seq_t)4294967295ULL;

/* compute the n‑th term of A384404, handling any order of calls    */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined, safe fallback */

    /* initialise cache on first use                                 */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 2);  /* indices 0,1 */
        if (cache == NULL) return 0;               /* allocation failure */
        cache[1] = (seq_t)1;                       /* a(1) = 1 */
        cache_max = 1;
    }

    /* already computed? */
    if (n <= cache_max) return cache[n];

    /* grow the cache to size n+1 (index 0 unused) */
    seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));
    if (tmp == NULL) return 0;                          /* allocation failure */
    cache = tmp;

    /* fill missing terms using the recurrence */
    seq_t prev = cache[cache_max];
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        prev = (prev * (seq_t)134775813 + (seq_t)1) & MODMASK;
        cache[i] = prev;
    }
    cache_max = n;
    return cache[n];
}
