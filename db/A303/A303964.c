/*
 * A303964  Number of nX3 0..1 arrays with every element unequal to 0, 1, 3 or
 *          5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) + 2*a(n-4) + a(n-5) + a(n-6) - a(n-7) -
 *            a(n-11) - a(n-12) Initial values (for 1 ≤ n ≤ 12): a(1)=3,
 *            a(2)=9, a(3)=13, a(4)=26, a(5)=54, a(6)=101, a(7)=186, a(8)=361,
 *            a(9)=700, a(10)=1340, a(11)=2562, a(12)=4913
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 33, u64 n <= 67, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t last  = 0;

    /* Allocate or grow the cache if needed */
    if (cap < n + 1) {
        void *tmp = realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        cache = (seq_t *)tmp;
        cap   = n + 1;
    }

    /* Compute new values up to n */
    static const uint64_t init64[13] = {
        0ULL, /* unused 0 index */
        3ULL, 9ULL, 13ULL, 26ULL, 54ULL,
        101ULL, 186ULL, 361ULL, 700ULL,
        1340ULL, 2562ULL, 4913ULL
    };

    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i <= 12) {
                cache[i] = (seq_t)init64[i];
            } else {
                cache[i] = cache[i-1]
                         + 2ULL * cache[i-3]
                         + 2ULL * cache[i-4]
                         + cache[i-5]
                         + cache[i-6]
                         - cache[i-7]
                         - cache[i-11]
                         - cache[i-12];
            }
        }
        last = n;
    }

    return cache[n];
}
