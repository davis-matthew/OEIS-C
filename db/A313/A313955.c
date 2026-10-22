/*
 * A313955  Coordination sequence Gal.4.144.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) - a(n-6) + a(n-7) + a(n-9) -
 *            a(n-10), with a(0)=1, a(1)=5, a(2)=10, a(3)=17, a(4)=21,
 *            a(5)=26, a(6)=32, a(7)=38, a(8)=43, a(9)=47, a(10)=54
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* Initialize newly allocated slots */
        for (idx_t i = cache_cap; i < new_cap; ++i) {
            if (i == 0)                 cache[i] = (seq_t)1;
            else if (i == 1)            cache[i] = (seq_t)5;
            else if (i == 2)            cache[i] = (seq_t)10;
            else if (i == 3)            cache[i] = (seq_t)17;
            else if (i == 4)            cache[i] = (seq_t)21;
            else if (i == 5)            cache[i] = (seq_t)26;
            else if (i == 6)            cache[i] = (seq_t)32;
            else if (i == 7)            cache[i] = (seq_t)38;
            else if (i == 8)            cache[i] = (seq_t)43;
            else if (i == 9)            cache[i] = (seq_t)47;
            else if (i == 10)           cache[i] = (seq_t)54;
            else {
                seq_t val = cache[i-1] + cache[i-3];
                val -= cache[i-4];
                val -= cache[i-6];
                val += cache[i-7];
                val += cache[i-9];
                val -= cache[i-10];
                cache[i] = val;
            }
        }
        cache_cap = new_cap;
    }
    return cache[n];
}
