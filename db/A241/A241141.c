/*
 * A241141  Numbers n such that n and 5n share at least one digit.
 * https://oeis.org/A241141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   share_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A241141: numbers n such that n and 5n share at least one decimal digit. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/share_digit.h"

static seq_t *cache = NULL;   /* cached terms */
static size_t cacheCap = 0;   /* capacity of the cache */
static size_t cacheSize = 0;  /* number of terms stored */
static seq_t lastChecked = 0; /* last integer examined */

/* Grow the cache when necessary. */
static void grow_cache(void)
{
    size_t newCap = (cacheCap == 0) ? 4 : cacheCap * 2;
    seq_t *newCache = (seq_t *)malloc(newCap * sizeof(seq_t));
    if (!newCache) return; /* allocation failed – caller will not get more terms */
    if (cache) {
        memcpy(newCache, cache, cacheSize * sizeof(seq_t));
        free(cache);
    }
    cache = newCache;
    cacheCap = newCap;
}

/* Return the n-th term of the sequence (a(1) = 5, a(2) = 10, …). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* behaviour below offset is undefined */

    /* Keep generating terms until we have at least n cached. */
    while (cacheSize < (size_t)n) {
        ++lastChecked;                         /* next candidate */
        seq_t five = lastChecked * (seq_t)5;    /* 5 * candidate */
        if (share_digit(lastChecked, five)) {
            if (cacheSize == cacheCap) grow_cache();
            cache[cacheSize++] = lastChecked;
        }
    }
    return cache[n - 1];
}
