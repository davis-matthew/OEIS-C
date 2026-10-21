/*
 * A275141  Number of n X 7 0..2 arrays with no element equal to any value at
 *          offset (-2,-2) (-1,-2) or (0,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 32
   a(2) = 1296
   a(3) = 2816
   a(4) = 17872
   a(5) = 107472
   a(6) = 663904
   a(7) = 4055152
   For n > 7
     a(n) = 3*a(n-1) + 18*a(n-2) + 11*a(n-3) - 23*a(n-4) - 4*a(n-5)
*/

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic table of computed values   */
static idx_t cache_len = 0;      /* allocated size (always > max index) */
static idx_t max_computed = 0;   /* highest index for which a value is stored */

/* initialize base values */
static void init_cache(void)
{
    cache_len = 8;                    /* indices 0..7 */
    cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    cache[0] = (seq_t)0;              /* unused, keep zero */
    cache[1] = (seq_t)32;
    cache[2] = (seq_t)1296;
    cache[3] = (seq_t)2816;
    cache[4] = (seq_t)17872;
    cache[5] = (seq_t)107472;
    cache[6] = (seq_t)663904;
    cache[7] = (seq_t)4055152;
    max_computed = 7;
}

/* grow cache so that it contains indices up to n */
static void ensure_cache(idx_t n)
{
    if (n <= max_computed)
        return;

    /* enlarge allocation if necessary */
    if (cache_len <= n) {
        size_t new_len = cache_len == 0 ? 8 : cache_len;
        while (new_len <= n) new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;
        cache_len = new_len;
    }

    /* compute missing terms */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* recurrence only valid for i > 7 */
        seq_t val = (seq_t)3 * cache[i-1]
                  + (seq_t)18 * cache[i-2]
                  + (seq_t)11 * cache[i-3];
        val -= (seq_t)23 * cache[i-4];
        val -= (seq_t)4  * cache[i-5];
        cache[i] = val;
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (!cache)
        init_cache();

    ensure_cache(n);
    return cache[n];
}
