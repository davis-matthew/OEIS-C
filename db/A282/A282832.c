/*
 * A282832  Number of nX2 0..1 arrays with no 1 equal to more than one of its
 *          king-move neighbors, with the exception of exactly two elements.
 * https://oeis.org/A282832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +6*a(n-2) -5*a(n-3) -42*a(n-4) -33*a(n-5)
 *            +51*a(n-6) +156*a(n-7) +144*a(n-8) +64*a(n-9); Initial values:
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=16, a(5)=32, a(6)=144, a(7)=544,
 *            a(8)=1664, a(9)=5664
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* highest index computed so far */

    if (cache == NULL) {
        /* initial cache of size 9 (index 0 unused) */
        cache_len = 9;
        cache = alloc_seq_array(cache_len + 1);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)16;
        cache[5] = (seq_t)32;
        cache[6] = (seq_t)144;
        cache[7] = (seq_t)544;
        cache[8] = (seq_t)1664;
        cache[9] = (seq_t)5664;
    }

    if (n <= cache_len) {
        return cache[n];
    }

    /* Grow the cache to hold indices up to n */
    seq_t *new_cache = alloc_seq_array(n + 1);
    for (idx_t i = 0; i <= cache_len; ++i) {
        new_cache[i] = cache[i];
    }

    /* Compute new entries using the 9‑term recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = 3 * new_cache[i-1] + 6 * new_cache[i-2];
        val -= 5 * new_cache[i-3];
        val -= 42 * new_cache[i-4];
        val -= 33 * new_cache[i-5];
        val += 51 * new_cache[i-6];
        val += 156 * new_cache[i-7];
        val += 144 * new_cache[i-8];
        val += 64 * new_cache[i-9];
        new_cache[i] = val;
    }

    free_seq_array(cache);
    cache = new_cache;
    cache_len = n;

    return cache[n];
}
