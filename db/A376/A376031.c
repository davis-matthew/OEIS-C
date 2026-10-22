/*
 * A376031  Number of ways to tile a 3 x (2*n) rectangle with dominoes and
 *          T's.
 * https://oeis.org/A376031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 7*a(n-2) + 4*a(n-3) - 8*a(n-4) + 2*a(n-5)
 *            Initial values: a(0)=1, a(1)=3, a(2)=18, a(3)=112, a(4)=692,
 *            a(5)=4294
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define INIT_LEN 6

static seq_t *cache = NULL;      /* dynamic array of computed terms */
static idx_t  cache_alloc = 0;   /* size of allocated array */
static idx_t  cache_last  = 0;   /* number of terms actually computed */

seq_t a(idx_t n)
{
    /* fixed initial terms (possibly truncated if seq_t is small) */
    static const seq_t init_vals[INIT_LEN] = {1, 3, 18, 112, 692, 4294};

    /* First allocation and initialization of base values */
    if (cache == NULL) {
        idx_t size = INIT_LEN;
        if (size <= n) size = n + 1;          /* at least up to n or INIT_LEN */
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);

        for (idx_t i = 0; i < INIT_LEN; ++i)
            cache[i] = init_vals[i];

        cache_alloc = size;
        cache_last  = INIT_LEN;               /* we have first INIT_LEN terms */
    }

    /* If the requested term is already computed, return it directly */
    if (n < cache_last)
        return cache[n];

    /* Grow the array if necessary */
    if (cache_alloc <= n) {
        idx_t new_alloc = cache_alloc ? cache_alloc : 1;
        while (new_alloc <= n) new_alloc <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_alloc = new_alloc;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_last; i <= n; ++i) {
        if (i < (idx_t)INIT_LEN) {
            cache[i] = init_vals[i];
            continue;
        }
        /* Positive part: 5*a(n-1) + 7*a(n-2) + 4*a(n-3) + 2*a(n-5) */
        seq_t pos = 5 * cache[i - 1];
        pos += 7 * cache[i - 2];
        pos += 4 * cache[i - 3];
        pos += 2 * cache[i - 5];
        /* Subtract the negative part */
        pos -= 8 * cache[i - 4];
        cache[i] = pos;
    }

    cache_last = n + 1;    /* all terms up to n are now available */
    return cache[n];
}
