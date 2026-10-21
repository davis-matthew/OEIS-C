/*
 * A279077  Maximum starting value of X such that repeated replacement of X
 *          with X-ceiling(X/7) requires n steps to reach 0.
 * https://oeis.org/A279077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(a(n-1)*7/6)+1, a(0)=0
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The function returns the nth term of OEIS A279077.
 * It uses lazy memoisation so that calls can be in any order.
 * The implementation works for both the fixed‑width integer builds
 * (seq_t as uint8_t, uint16_t, uint32_t, uint64_t, unsigned __int128)
 * and for the arbitrary‑precision build (seq_t as a C++ big integer class).
 */

static seq_t *cache = NULL;   /* memoisation array */
static idx_t max_computed = 0; /* largest n that has been computed */

seq_t a(idx_t n)
{
    /* Handle the very first call.  Allocate an array big enough for n. */
    if (cache == NULL) {
        size_t sz = (size_t)(n + 1);
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) abort();          /* out of memory */
        cache[0] = 0;                /* a(0) = 0 */
        max_computed = 0;
    }

    /* If we already have a value for this n, just return it. */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Grow the cache sufficiently large for the new value.  We
     * reallocate to exactly n+1 elements – this keeps the code simple
     * while still being asymptotically optimal for repeated calls. */
    size_t newsize = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!tmp) abort();              /* out of memory */
    cache = tmp;

    /* Compute missing terms using the recurrence.  All arithmetic is
     * unsigned and uses only the allowed operators. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = prev * 7 / 6 + 1;    /* floor(prev*7/6)+1 */
    }

    max_computed = n;
    return cache[n];
}
