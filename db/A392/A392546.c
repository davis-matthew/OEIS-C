/*
 * A392546  Expansion of 1 / ((1-x)^5 - x^7).
 * https://oeis.org/A392546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 35, u64 n <= 73, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) + a(n-7)
   Initial values: a(0)=1, a(1)=5, a(2)=15, a(3)=35, a(4)=70, a(5)=126, a(6)=210  */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;     /* capacity of cache array */
static size_t computed_max = 0;  /* highest index computed (inclusive) */

seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n < 7) {
        static const seq_t init[7] = {1, 5, 15, 35, 70, 126, 210};
        return init[n];
    }

    /* Ensure we have a cache big enough for n + 1 entries */
    if (!cache) {
        cache_cap = (size_t)n + 1;
        if (cache_cap < 64) cache_cap = 64;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        /* Initialize known terms */
        cache[0] = 1;
        cache[1] = 5;
        cache[2] = 15;
        cache[3] = 35;
        cache[4] = 70;
        cache[5] = 126;
        cache[6] = 210;
        computed_max = 6;
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n * 2 + 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Ensure initial terms are set if we had a partial cache */
    if (computed_max < 6) {
        cache[0] = 1;
        cache[1] = 5;
        cache[2] = 15;
        cache[3] = 35;
        cache[4] = 70;
        cache[5] = 126;
        cache[6] = 210;
        computed_max = 6;
    }

    /* Compute missing values up to n */
    for (idx_t i = (idx_t)(computed_max + 1); i <= n; ++i) {
        seq_t pos = (seq_t)5 * cache[i - 1]
                  + (seq_t)10 * cache[i - 3]
                  + cache[i - 5]
                  + cache[i - 7];
        seq_t neg = (seq_t)10 * cache[i - 2]
                  + (seq_t)5 * cache[i - 4];
        cache[i] = pos - neg;  /* pos >= neg by recurrence definition */
    }
    computed_max = (size_t)n;

    return cache[n];
}
