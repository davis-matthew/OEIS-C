/*
 * A284663  Number of dominating sets in the Moebius ladder M_n.
 * https://oeis.org/A284663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1)+a(n-2)+5*a(n-3)+a(n-4)+a(n-5)-a(n-6)-a(n-7) for
 *            n>7 Initial values: a(1)=3, a(2)=15, a(3)=51, a(4)=179,
 *            a(5)=663, a(6)=2439, a(7)=8935
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static const seq_t init[7] = { (seq_t)3, (seq_t)15, (seq_t)51, (seq_t)179, (seq_t)663, (seq_t)2439, (seq_t)8935 };

static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;
    size_t new_size = (size_t)n;
    cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    size_t init_end = new_size < 7 ? new_size : 7;
    for (size_t i = cache_size; i < init_end; ++i) {
        cache[i] = init[i];
    }
    for (size_t i = init_end; i < new_size; ++i) {
        cache[i] = (seq_t)3 * cache[i-1] +
                   cache[i-2] +
                   (seq_t)5 * cache[i-3] +
                   cache[i-4] +
                   cache[i-5] -
                   cache[i-6] -
                   cache[i-7];
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return cache[n-1];
}
