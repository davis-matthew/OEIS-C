/*
 * A282879  Number of n X 2 0..1 arrays with no 1 equal to more than one of
 *          its horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A282879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Empirical recurrence (OEIS A282879)
   a(1) = 0
   a(2) = 2
   a(3) = 8
   a(4) = 32
   a(5) = 122
   a(6) = 416
   a(7) = 1414
   a(8) = 4626
   a(9) = 14930
   a(10) = 47432
   a(n) = 2*a(n-1) + 7*a(n-2) - 2*a(n-3) - 20*a(n-4) - 24*a(n-5)
          - 19*a(n-6) - 14*a(n-7) - 7*a(n-8) - 2*a(n-9) - a(n-10)   (n > 10)
   The function caches computed values so that calls can arrive in any order.
*/

#include <stddef.h>
#include <stdlib.h>

/* Static cache for computed terms.  Indices are 1‑based so
   cache[0] is unused.  Cache[1]..cache[cache_len] contain valid terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* current capacity (number of slots) */
static size_t cache_len = 0;   /* highest index that has been computed */

/* Initialise the cache with the first 10 known terms. */
static void init_cache(void)
{
    if (cache != NULL)
        return;                 /* already initialised */

    cache_cap = 11;             /* indices 0..10 inclusive */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (cache == NULL)
        abort();                /* fatal error */

    /* cache[0] is unused but set to 0 for safety */
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)8;
    cache[4] = (seq_t)32;
    cache[5] = (seq_t)122;
    cache[6] = (seq_t)416;
    cache[7] = (seq_t)1414;
    cache[8] = (seq_t)4626;
    cache[9] = (seq_t)14930;
    cache[10] = (seq_t)47432;
    cache_len = 10;             /* terms 1..10 are now stored */
}

/* Extend the cache to hold at least indices 0..n.  The size is
   doubled each time it is insufficient; this guarantees amortised
   O(1) growth. */
static void ensure_capacity(size_t n)
{
    if (n < cache_cap)
        return;                 /* already large enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();                /* fatal error */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) for n >= 1 (offset 1).  The function is safe for
   arbitrary order of calls. */
seq_t a(idx_t n)
{
    if (n == 0)            /* defensive: outside defined range */
        return (seq_t)0;

    init_cache();

    size_t pos = (size_t)n;
    if (pos <= cache_len)
        return cache[pos];

    /* Grow the cache if needed */
    ensure_capacity(pos);

    /* Compute the missing terms one by one */
    for (size_t i = cache_len + 1; i <= pos; ++i)
    {
        seq_t val = 2 * cache[i-1]
                  + 7 * cache[i-2]
                  - 2 * cache[i-3]
                  - 20 * cache[i-4]
                  - 24 * cache[i-5]
                  - 19 * cache[i-6]
                  - 14 * cache[i-7]
                  - 7  * cache[i-8]
                  - 2  * cache[i-9]
                  -    cache[i-10];
        cache[i] = val;
    }
    cache_len = pos;
    return cache[pos];
}
