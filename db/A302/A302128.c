/*
 * A302128  a(n) = a(a(n-2)) + a(n-a(n-1)) with a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A302128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(a(n-2)) + a(n-a(n-1)) with initial values
 *            a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation.  The size 100000 is enough for all
   tests mentioned in the problem statement (up to about n = 10000). */
#define CACHE_CAPACITY 100000

static seq_t cache[CACHE_CAPACITY + 1];
static idx_t cached_until = 0;   /* largest n already computed */

/* initialise base cases */
static void init_cache(void)
{
    cache[1] = 1;
    cache[2] = 1;
    cache[3] = 1;
    cached_until = 3;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety guard, OEIS range starts at 1 */

    /* initialise cache on first use */
    if (cached_until == 0) {
        init_cache();
    }

    if (n > CACHE_CAPACITY) {
        /* out of bounds – for safety we abort as the sequence is
           not defined beyond the cache capacity requested in the test. */
        abort();
    }

    if (n > cached_until) {
        /* compute from cached_until+1 up to n */
        for (idx_t i = cached_until + 1; i <= n; ++i) {
            /* indices accessed are always < i, hence already cached */
            idx_t idx1 = i - (idx_t)cache[i - 1];
            idx_t idx2 = (idx_t)cache[i - 2];

            /* the recursive definition: a(i) = a(a(i-2)) + a(i - a(i-1)) */
            cache[i] = cache[idx2] + cache[idx1];
        }
        cached_until = n;
    }

    return cache[n];
}
