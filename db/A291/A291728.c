/*
 * A291728  p-INVERT of (1,0,1,0,0,0,0,...), where p(S) = 1 - S - S^2.
 * https://oeis.org/A291728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A291728
 * a(0)=1, a(1)=2, a(2)=4, a(3)=9, a(4)=17, a(5)=35, a(6)=70
 * a(n)=a(n-1)+a(n-2)+a(n-3)+2*a(n-4)+a(n-6)  for n≥7
 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = (seq_t *)0;      /* pointer to computed values */
static idx_t cache_size = 0;           /* number of entries currently stored */

/* Ensure the cache is large enough and fill missing entries */
static void ensure_cache(idx_t n)
{
    if (!cache) {
        /* initialise with the first 7 terms */
        cache_size = 7;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 4;
        cache[3] = 9;
        cache[4] = 17;
        cache[5] = 35;
        cache[6] = 70;
    }

    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;

        /* compute new terms using the recurrence */
        for (idx_t i = old_size; i <= n; ++i) {
            if (i >= 7) {
                cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                         + (cache[i-4] * 2)
                         + cache[i-6];
            }
            /* values for i<7 are already initialised */
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
