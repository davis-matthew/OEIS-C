/*
 * A347730  Number of compositions (ordered partitions) of n into at most 2
 *          triangular numbers.
 * https://oeis.org/A347730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (isTriangular(n) ? 1 : 0) + * Σ_{k=1}^{n-1}
 *            isTriangular(k) * isTriangular(n‑k)
 * Range:     u8 n <= 7, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t count = 0;

    /* Check if n itself is triangular: n = t_k = k(k+1)/2 */
    seq_t s = (seq_t)8 * n + 1;
    seq_t sq = floor_sqrt(s);
    if (sq * sq == s) count += 1;

    /* Count ordered pairs (t1, t2) of positive triangular numbers with
     * t1 + t2 = n.  We iterate over t1 < n; then t2 = n - t1 > 0. */
    for (idx_t j = 1; ; ++j)
    {
        seq_t t1 = ((seq_t)j * (j + 1)) / 2;
        if (t1 >= n) break;          /* only positive t2 allowed */

        seq_t rem = n - t1;           /* t2 */
        seq_t s2 = (seq_t)8 * rem + 1;
        seq_t sq2 = floor_sqrt(s2);
        if (sq2 * sq2 == s2) count += 1;
    }

    return count;
}
