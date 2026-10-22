/*
 * A313884  Coordination sequence Gal.6.347.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - 2*a(n-10) +
 *            2*a(n-11) - a(n-12) for n > 12 Initial terms: a(0)=1, a(1)=5,
 *            a(2)=10, a(3)=16, a(4)=21, a(5)=27, a(6)=31, a(7)=35, a(8)=41,
 *            a(9)=46, a(10)=52, a(11)=57, a(12)=62
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Initialize cache with base terms if not already done */
    if (cache == NULL) {
        size_t init = 13; /* indices 0..12 inclusive */
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure – unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)27;
        cache[6] = (seq_t)31;
        cache[7] = (seq_t)35;
        cache[8] = (seq_t)41;
        cache[9] = (seq_t)46;
        cache[10] = (seq_t)52;
        cache[11] = (seq_t)57;
        cache[12] = (seq_t)62;
        cache_size = init;
    }

    if (n < cache_size) {
        return cache[n];
    }

    /* Extend cache up to n */
    size_t new_size = (size_t)n + 1;
    if (new_size > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure – unlikely */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = 2 * cache[i - 1];
            val -= 2 * cache[i - 2];
            val += 2 * cache[i - 3];
            val -= 2 * cache[i - 4];
            val += 2 * cache[i - 5];
            val -= 2 * cache[i - 6];
            val += 2 * cache[i - 7];
            val -= 2 * cache[i - 8];
            val += 2 * cache[i - 9];
            val -= 2 * cache[i - 10];
            val += 2 * cache[i - 11];
            val -= cache[i - 12];
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n];
}
