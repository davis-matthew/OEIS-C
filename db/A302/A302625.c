/*
 * A302625  Number of 4Xn 0..1 arrays with every element equal to 0, 1, 2 or 6
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 54, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A302625
 * Recurrence (n > 11):
 *   a(n) = a(n-1) + 2*a(n-2) + 2*a(n-3) + a(n-4)
 *          - 4*a(n-5) - a(n-6) - 2*a(n-7) + 2*a(n-8)
 * Initial values:
 *   a(1)=8, a(2)=81, a(3)=71, a(4)=130, a(5)=251,
 *   a(6)=497, a(7)=1051, a(8)=2274, a(9)=4999,
 *   a(10)=10972, a(11)=24236
 *
 * The implementation caches computed values for random access
 * and grows the cache with realloc.  All integer arithmetic uses
 * seq_t, which may be an 8/16/32/64-bit unsigned type or an
 * arbitrary‑precision integer class.  The code is valid C11
 * and C++17.  It does not rely on any external helpers.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for indices < 1 */
        return 0u;      /* use the constant's type to avoid warnings */

    /* Cache: index 0 unused, cache[i] holds a(i) */
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;   /* capacity: number of elements allocated */
    static idx_t  computed   = 0;   /* highest index already computed */

    /* Grow the cache if needed.  We allocate space for indices 0..n. */
    if ((idx_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1) * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap);
        if (!tmp) return 0u;         /* allocation failure (unlikely) */
        cache = tmp;
        cache_cap = (idx_t)(n + 1);
    }

    /* Compute missing values up to n. */
    if (computed < n) {
        /* Initial values for n = 1 .. 11 */
        static const seq_t base[12] = {
            0u,                      /* dummy for index 0 */
            8u, 81u, 71u, 130u, 251u, 497u,
            1051u, 2274u, 4999u,
            10972u, 24236u
        };

        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i <= 11) {
                cache[i] = base[i];
            } else {
                /* recurrence */
                seq_t t = cache[i - 1];
                t += cache[i - 2] + cache[i - 2];
                t += cache[i - 3] + cache[i - 3];
                t += cache[i - 4];
                t -= cache[i - 5] + cache[i - 5] + cache[i - 5] + cache[i - 5];
                t -= cache[i - 6];
                t -= cache[i - 7] + cache[i - 7];
                t += cache[i - 8] + cache[i - 8];
                cache[i] = t;
            }
        }
        computed = n;
    }

    return cache[n];
}
