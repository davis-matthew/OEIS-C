/*
 * A313969  Coordination sequence Gal.6.489.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) - a(n-2) - a(n-3) + a(n-4) + a(n-5) - 2*a(n-6)
 *            + 2*a(n-7) - 2*a(n-8) + a(n-9) + a(n-10) - a(n-11) - a(n-12)
 *            + 2*a(n-13) - a(n-14)   for n > 14
 * Initial values:
 * a(0)=1, a(1)=5, a(2)=10, a(3)=17, a(4)=21,
 * a(5)=28, a(6)=34, a(7)=38, a(8)=43, a(9)=47,
 * a(10)=54, a(11)=59, a(12)=66, a(13)=73, a(14)=76
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached sequence values */
    static size_t  cap   = 0;     /* allocated capacity */
    static size_t  sz    = 0;     /* number of values currently stored */

    /* Return cached value if available */
    if ((size_t)n < sz)
        return cache[(size_t)n];

    /* Ensure enough space for all indices up to n */
    size_t need = (size_t)n + 1;
    if (cap < need) {
        size_t newcap = cap ? cap << 1 : 16;
        while (newcap < need)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            abort();  /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* Initial values for indices 0 … 14 */
    static const seq_t base[15] = {
        (seq_t)1,  (seq_t)5,  (seq_t)10, (seq_t)17, (seq_t)21,
        (seq_t)28, (seq_t)34, (seq_t)38, (seq_t)43, (seq_t)47,
        (seq_t)54, (seq_t)59, (seq_t)66, (seq_t)73, (seq_t)76
    };

    if (sz < 15) {
        size_t start = sz;
        size_t end   = (n < 14) ? (size_t)n : 14;
        for (size_t i = start; i <= end; ++i)
            cache[i] = base[i];
        sz = (n < 14) ? (size_t)n + 1 : 15;
    }

    /* Compute remaining terms using the recurrence */
    for (size_t i = sz; i <= (size_t)n; ++i) {
        seq_t val = 2 * cache[i - 1]
                    - cache[i - 2]
                    - cache[i - 3]
                    + cache[i - 4]
                    + cache[i - 5]
                    - 2 * cache[i - 6]
                    + 2 * cache[i - 7]
                    - 2 * cache[i - 8]
                    + cache[i - 9]
                    + cache[i - 10]
                    - cache[i - 11]
                    - cache[i - 12]
                    + 2 * cache[i - 13]
                    - cache[i - 14];
        cache[i] = val;
    }
    sz = (size_t)n + 1;

    return cache[(size_t)n];
}
