/*
 * A315913  Coordination sequence Gal.6.599.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-8) - a(n-10) for n > 14; a(0)=1, a(1)=6,
 *            a(2)=8, a(3)=14, a(4)=20, a(5)=26, a(6)=32, a(7)=38, a(8)=36,
 *            a(9)=54, a(10)=48, a(11)=62, a(12)=58, a(13)=74, a(14)=68,
 *            a(15)=86, a(16)=72, a(17)=102, a(18)=84, a(19)=110, a(20)=94,
 *            a(21)=122, a(22)=104, a(23)=134
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache and bookkeeping */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = (idx_t)-1;

    /* small initial cache with known terms (0..23) */
    if (cache_cap == 0) {
        /* reserve small capacity: enough for the initial terms */
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* out‑of‑memory guard */

        /* initialise known values */
        cache[0]  = 1;   cache[1]  = 6;   cache[2]  = 8;
        cache[3]  = 14;  cache[4]  = 20;  cache[5]  = 26;
        cache[6]  = 32;  cache[7]  = 38;  cache[8]  = 36;
        cache[9]  = 54;  cache[10] = 48;  cache[11] = 62;
        cache[12] = 58;  cache[13] = 74;  cache[14] = 68;
        cache[15] = 86;  cache[16] = 72;  cache[17] = 102;
        cache[18] = 84;  cache[19] = 110; cache[20] = 94;
        cache[21] = 122; cache[22] = 104; cache[23] = 134;

        last_computed = 23;
    }

    /* if the requested value is already computed, return it */
    if (n <= last_computed)
        return cache[(size_t)n];

    /* extend cache up to the requested index */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* resize if needed */
        if ((size_t)i >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* out‑of‑memory guard */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* apply the recurrence for n > 14 */
        if (i > 14) {
            cache[(size_t)i] =
                cache[(size_t)(i - 2)] + cache[(size_t)(i - 8)] - cache[(size_t)(i - 10)];
        }
    }

    last_computed = n;
    return cache[(size_t)n];
}
