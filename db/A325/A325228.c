/*
 * A325228  Number of integer partitions of n such that the lesser of the
 *          maximum part and the number of parts is 3.
 * https://oeis.org/A325228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n with min(max part, number of
 *            parts) = 3. Computed as: a(n) = [p3(n) - (3≤n≤6)] + [q3(n) -
 *            (n=3 or 4≤n≤6)] - C(n), where p3(n) = floor((n^2 + 3)/12)
 *            (partitions into exactly 3 parts) q3(n) =
 *            sum_{z=0}^{⌊n/3⌋}(⌊(n-3z)/2⌋ + 1) - (⌊n/2⌋ + 1) (partitions with
 *            max part...
 * Range:     u8 n <= 39, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;  /* maximum n that has been computed */

    if (n <= cached_n && cache != NULL) return cache[n];

    /* extend cache if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    } else if (n > cached_n) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    }

    /* compute values from cached_n + 1 up to n */
    for (idx_t k = cached_n + 1; k <= n; ++k) {
        /* Handle offset (a(1)=a(4)=0) */
        if (k < 5) {
            cache[k] = 0;
            continue;
        }

        /* ---- compute p3(k): partitions into exactly 3 parts ---- */
        seq_t p3 = ((seq_t)k * (seq_t)k + 3) / 12;

        /* subtract partitions into 3 parts with all parts ≤2 (only for k=3..6) */
        seq_t adjust_p3 = ((k >= 3 && k <= 6) ? 1 : 0);
        seq_t A = p3 - adjust_p3;  /* r = 3, max part ≥3 */

        /* ---- compute q3(k): partitions with max part exactly 3 ---- */
        seq_t p_le3 = 0;
        for (idx_t z = 0; z <= k / 3; ++z) {
            idx_t r = k - 3 * z;
            p_le3 += (r / 2 + 1);
        }
        seq_t p_le2 = (k / 2) + 1;                 /* partitions with parts ≤2 */
        seq_t q3 = p_le3 - p_le2;                  /* partitions with max part 3 */

        /* subtract partitions with max part 3 but number of parts < 3
           (r = 1 if k==3; r = 2 if k=4,5,6) */
        seq_t adjust_q3 = ((k == 3) || (k >= 4 && k <= 6)) ? 1 : 0;
        seq_t B = q3 - adjust_q3;                  /* max part = 3, parts ≥3 */

        /* ---- compute intersection C(k): partitions with 3 parts and max part 3 ---- */
        seq_t C = 0;
        for (idx_t z = 1; z <= 3; ++z) {
            if (k >= 3 + 2 * z) {
                idx_t y = k - 3 - 2 * z;
                if (y <= 3 - z) {
                    ++C;
                }
            }
        }

        cache[k] = A + B - C;
    }

    cached_n = n;
    return cache[n];
}
