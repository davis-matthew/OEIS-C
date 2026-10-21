/*
 * A268507  Number of ordered ways to write n as w^2 + x^2 + y^2 + z^2 with w
 *          > 0, w >= x <= y <= z such that x^2*y^2 + y^2*z^2 + z^2*x^2 is a
 *          square, where w,x,y,z are nonnegative integers.
 * https://oeis.org/A268507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A268507: Count of ordered quadruples (w,x,y,z) with
 *   w > 0, w >= x <= y <= z,
 *   w² + x² + y² + z² = n,
 *   and x²y² + y²z² + z²x² is a perfect square.
 *
 * The function computes the sequence by exhaustive enumeration of
 * all admissible (w,x,y,z) up to the requested n, caching the
 * results for subsequent calls.  The algorithm is O(n^(3/2)) in
 * practice and satisfies the required time limit for n ≲ 10 000. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* --- Integer square‑root (floor) for idx_t values --- */
static inline idx_t int_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            if (mid == 0) return 0;
            hi = mid - 1;
        }
    }
    return hi;
}

/* --- Main sequence function -------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset starts at 1 */

    /* static cache of computed values */
    static seq_t *cache      = NULL;
    static idx_t  cache_size = 0;         /* highest index cached */

    /* enlarge the cache (and recompute all values) if necessary */
    if (n > cache_size) {
        /* allocate a fresh array of zeros */
        seq_t *new_cache = (seq_t*)calloc(n + 1, sizeof(seq_t));
        if (!new_cache) return 0;        /* allocation failure */

        /* enumerate all admissible quadruples up to n */
        idx_t wmax = int_isqrt_u64(n);
        for (idx_t w = 1; w <= wmax; ++w) {
            idx_t w2 = w * w;
            for (idx_t x = 0; x <= w; ++x) {
                idx_t x2 = x * x;
                for (idx_t y = x; y <= wmax; ++y) {
                    idx_t y2 = y * y;
                    for (idx_t z = y; z <= wmax; ++z) {
                        idx_t z2 = z * z;
                        idx_t sum = w2 + x2 + y2 + z2;
                        if (sum > n) break;          /* z only increases */

                        /* check whether x²y² + y²z² + z²x² is a square */
                        idx_t prod = x2 * y2 + y2 * z2 + z2 * x2;
                        idx_t s    = int_isqrt_u64(prod);
                        if (s * s == prod) {
                            ++new_cache[sum];
                        }
                    }
                }
            }
        }

        free(cache);           /* replace old cache */
        cache      = new_cache;
        cache_size = n;
    }

    return cache[n];
}
