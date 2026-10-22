/*
 * A311094  Coordination sequence Gal.6.32.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311094
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

/* 
   OEIS sequence A311094 – Coordination sequence Gal.6.32.5
   Formula: a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) - a(n-6) 
             + a(n-7) + a(n-8) - a(n-9) + a(n-10) - a(n-11) + a(n-12)
             - a(n-13) + a(n-14) - a(n-15)  for n ≥ 15
   Initial values:
     a(0)=1, a(1)=4, a(2)=8, a(3)=12, a(4)=15, a(5)=18, a(6)=23,
     a(7)=27, a(8)=31, a(9)=35, a(10)=39, a(11)=43, a(12)=45,
     a(13)=48, a(14)=54, a(15)=58
*/

#include <stdlib.h>

static const seq_t init_vals[16] = {
    1, 4, 8, 12, 15, 18, 23, 27, 31, 35, 39, 43, 45, 48, 54, 58
};

seq_t a(idx_t n)
{
    /* Static cache to store computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Return the first 16 terms immediately */
    if (n < 16)
        return init_vals[n];

    /* Lazily allocate cache on first use */
    if (!cache) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;                     /* allocation failure */
        for (size_t i = 0; i < 16; ++i)
            cache[i] = init_vals[i];
    }

    /* Extend cache up to the requested index if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                     /* allocation failure */
        cache = tmp;
        for (idx_t i = (idx_t)cap; i <= n; ++i) {
            seq_t v = cache[i-1];
            v -= cache[i-2];
            v += cache[i-3];
            v -= cache[i-4];
            v += cache[i-5];
            v -= cache[i-6];
            v += cache[i-7];
            v += cache[i-8];
            v -= cache[i-9];
            v += cache[i-10];
            v -= cache[i-11];
            v += cache[i-12];
            v -= cache[i-13];
            v += cache[i-14];
            v -= cache[i-15];
            cache[i] = v;
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
