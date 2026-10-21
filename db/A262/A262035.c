/*
 * A262035  Number of permutations of [2n+1] beginning with exactly n ascents.
 * https://oeis.org/A262035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(n+1)*(2*n+1)*a(n-1)/(n+2); a(0) = 1, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;      /* memoisation array */
static idx_t cache_size = 0;     /* allocated length of cache array */

/* Grow the cache so it can hold indices up to n inclusive. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    idx_t new_size = cache_size == 0 ? 1 : cache_size;
    while (new_size <= n) new_size <<= 1;          /* double until big enough */

    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) return;                       /* out of memory: give up */

    /* copy old values */
    for (idx_t i = 0; i < cache_size; ++i) {
        new_cache[i] = cache[i];
    }
    /* initialise new cells to 0 (meaning "not yet computed") */
    for (idx_t i = cache_size; i < new_size; ++i) {
        new_cache[i] = 0;
    }

    if (cache) free(cache);
    cache = new_cache;
    cache_size = new_size;
}

/* Number of permutations of [2n+1] beginning with exactly n ascents */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* base cases */
    if (!cache[0]) cache[0] = 1;
    if (n == 0) return cache[0];
    if (!cache[1]) cache[1] = 2;
    if (n == 1) return cache[1];

    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i]) continue;          /* already computed */

        seq_t prev = cache[i - 1];

        seq_t num1   = (seq_t)2 * ((seq_t)i + 1);      /* 2*(i+1) */
        seq_t num2   = (seq_t)2 * (seq_t)i + 1;        /* 2*i+1 */
        seq_t denom  = (seq_t)i + 2;                  /* i+2 */

        /* Reduce by gcds to keep intermediate values reasonably small */
        seq_t g = gcd(prev, denom);
        prev   /= g;
        denom  /= g;

        g = gcd(num1, denom);
        num1  /= g;
        denom /= g;

        g = gcd(num2, denom);
        num2  /= g;
        denom /= g;                          /* denom should now be 1 */

        seq_t res = prev;
        res = res * num1;
        res = res * num2;
        cache[i] = res;
    }

    return cache[n];
}
