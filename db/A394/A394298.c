/*
 * A394298  a(n) is the number of unit cubes in the cubic lattice, divided by
 *          2, that have at least one vertex strictly inside and at least one
 *          vertex strictly outside the sphere (x-1/2)^2 + (y-1/2)^2 +
 *          (z-1/2)^2 = n, i.e., the sphere centered at the deep hole.
 * https://oeis.org/A394298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 6962, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Setup:
 *   a(n) = number of unit cubes in the lattice that have
 *   at least one vertex strictly inside and at least one vertex
 *   strictly outside the sphere (x-1/2)^2 + (y-1/2)^2 + (z-1/2)^2 = n,
 *   divided by two.
 *
 *  The algorithm enumerates all lattice cubes that can intersect the
 *  sphere for the required range of n, using the fact that a cube at
 *  indices (i,j,k) contributes to all integer radii n satisfying
 *
 *      S - U + 1 ≤ n ≤ S + U
 *
 *  where S = i^2 + j^2 + k^2 and U = |i| + |j| + |k|.
 *  The contributions are accumulated with a difference array, after
 *  which a prefix sum yields the total number of contributing cubes
 *  C(n).  The required sequence value is a(n) = C(n)/2.
 */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache for computed values up to 'max_cached' */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;

    if (max_cached <= n) {
        /* Compute up to new maximum n */
        /* Allocate cache for values a(0)..a(n)      */
        seq_t *new_cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failure – cannot compute */
            return (seq_t)0;
        }

        /* Difference array for accumulation of cube contributions  */
        int64_t *diff = (int64_t*)calloc((size_t)(n + 2), sizeof(int64_t));
        if (!diff) {
            free(new_cache);
            return (seq_t)0;
        }

        /* Upper bound on |i|, |j|, |k| needed to cover all cubes
           that can intersect the sphere of radius sqrt(n).                   */
        idx_t B = (idx_t)(sqrt((double)n) + 2.0);   /* safe over‑estimate */

        /* Enumerate all cubes with indices in [-B, B] */
        for (int64_t ii = -(int64_t)B; ii <= (int64_t)B; ++ii) {
            idx_t ai = (idx_t)(ii >= 0 ? ii : -(int64_t)ii);
            for (int64_t jj = -(int64_t)B; jj <= (int64_t)B; ++jj) {
                idx_t aj = (idx_t)(jj >= 0 ? jj : -(int64_t)jj);
                for (int64_t kk = -(int64_t)B; kk <= (int64_t)B; ++kk) {
                    idx_t ak = (idx_t)(kk >= 0 ? kk : -(int64_t)kk);

                    /* Skip the cube whose all vertices coincide (U==0) */
                    if (ai == 0 && aj == 0 && ak == 0)
                        continue;

                    /* Compute S = i^2 + j^2 + k^2  and  U = |i|+|j|+|k| */
                    int64_t S = (int64_t)ii * ii + (int64_t)jj * jj + (int64_t)kk * kk;
                    int64_t U = (int64_t)ai + (int64_t)aj + (int64_t)ak;

                    /* Compute contribution interval:  L = S-U+1,  R = S+U */
                    int64_t L = S - U + 1;
                    int64_t R = S + U;

                    /* Clip to [0, n] */
                    if (R < 0 || L > (int64_t)n)
                        continue;
                    if (L < 0) L = 0;
                    if (R > (int64_t)n) R = n;

                    /* Update difference array */
                    diff[(size_t)L] += 1;
                    diff[(size_t)(R + 1)] -= 1;
                }
            }
        }

        /* Prefix sum over diff to obtain total counts C(n) */
        int64_t running = 0;
        for (idx_t i = 0; i <= n; ++i) {
            running += diff[(size_t)i];
            new_cache[i] = (seq_t)(running / 2);  /* divide by two as per definition */
        }

        /* Replace old cache with the new one */
        free(cache);
        cache = new_cache;
        max_cached = n + 1;
        free(diff);
    }

    return cache[n];
}
