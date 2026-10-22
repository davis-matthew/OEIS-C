/*
 * A385036  Consecutive states of the linear congruential pseudo-random number
 *          generator (419*s + 6173) mod 29282 when started at s=1.
 * https://oeis.org/A385036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1 a(n) = (419 * a(n-1) + 6173) mod 29282 for n > 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[1] ... cache[last] */
static idx_t  cache_size = 0; /* highest index cached (0 means none) */

/* Return the n-th term of sequence A385036. */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1.  For n < 1 we
       return 0 – callers should never request such indices. */
    if (n < 1) return 0;

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));   /* indices 0 and 1 */
        if (cache == NULL) return 0;                /* allocation failure */
        cache[0] = 0;                               /* unused */
        cache[1] = 1;                               /* a(1) = 1 */
        cache_size = 1;
    }

    /* If requested value is already cached, return it. */
    if (n <= cache_size) return cache[n];

    /* Extend cache to accommodate up to index n. */
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (tmp == NULL) return 0;              /* allocation failure */
    cache = tmp;

    /* Compute missing terms using the recurrence. */
    const seq_t MOD = 29282;
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val = 419 * cache[i - 1] + 6173; /* intermediate fits in seq_t */
        cache[i] = val % MOD;
    }
    cache_size = n;

    return cache[n];
}
