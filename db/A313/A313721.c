/*
 * A313721  Coordination sequence Gal.5.129.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A313721
 *  Recurrence: a(0)=1, a(1)=5, a(2)=10, a(3)=15, a(4)=20, a(5)=24, a(6)=29,
 *              a(7)=34, a(8)=39, a(9)=44, a(10)=49,
 *              a(n) = a(n-1) + a(n-9) - a(n-10)   for n > 10
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache   = NULL;   /* array of already computed values   */
static size_t cache_len = 0;    /* number of valid entries in cache   */
static size_t cache_cap = 0;    /* current allocated capacity         */

/* enlarge cache so that it can hold values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    /* grow the allocated array if necessary */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* out of memory */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing entries */
    if (needed > cache_len) {
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            if (i <= 10) {
                /* predefined initial values */
                switch ((int)i) {
                case 0: cache[i] = (seq_t)1;  break;
                case 1: cache[i] = (seq_t)5;  break;
                case 2: cache[i] = (seq_t)10; break;
                case 3: cache[i] = (seq_t)15; break;
                case 4: cache[i] = (seq_t)20; break;
                case 5: cache[i] = (seq_t)24; break;
                case 6: cache[i] = (seq_t)29; break;
                case 7: cache[i] = (seq_t)34; break;
                case 8: cache[i] = (seq_t)39; break;
                case 9: cache[i] = (seq_t)44; break;
                case 10: cache[i] = (seq_t)49;break;
                }
            } else {
                /* recurrence for n > 10 */
                cache[i] = cache[i-1] + cache[i-9] - cache[i-10];
            }
        }
        cache_len = needed;
    }
}

/* external function returning a(n) */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
