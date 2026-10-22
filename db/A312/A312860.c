/*
 * A312860  Coordination sequence Gal.6.546.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) - a(n-6) +
 *            a(n-7) + a(n-8) - a(n-9) + a(n-10) - a(n-11) + a(n-12) - a(n-13)
 *            + a(n-14) - a(n-15) for n > 15 Initial values: a(0)=1, a(1)=4,
 *            a(2)=9, a(3)=12, a(4)=17, a(5)=21, a(6)=25, a(7)=30, a(8)=32,
 *            a(9)=38, a(10)=43, a(11)=44, a(12)=51, a(13)=5...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* The function must have external linkage. */
seq_t a(idx_t n)
{
    /* first 16 terms stored explicitly */
    static const seq_t init[16] = {
        1, 4, 9, 12, 17, 21, 25, 30,
        32, 38, 43, 44, 51, 55, 59, 64
    };

    /* static cache capable of holding all terms up to n = 10000
       (enough for the tests; can be enlarged if needed). */
    static seq_t cache[10001];
    static size_t cache_size = 0;   /* number of terms already filled */

    /* initialise cache once */
    if (!cache_size) {
        for (size_t i = 0; i < 16; ++i) cache[i] = init[i];
        /* remaining entries are undefined until computed */
        cache_size = 16;
    }

    if (n < cache_size) return cache[n];

    /* compute missing terms up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* recurrence uses a(i-1) .. a(i-15) */
        seq_t pos = cache[i-1]  + cache[i-3]  + cache[i-5]  + cache[i-7]
                   + cache[i-8]  + cache[i-10] + cache[i-12] + cache[i-14];
        seq_t neg = cache[i-2]  + cache[i-4]  + cache[i-6]  + cache[i-9]
                   + cache[i-11] + cache[i-13] + cache[i-15];
        cache[i] = pos - neg;
    }
    cache_size = (size_t)n + 1;
    return cache[n];
}
