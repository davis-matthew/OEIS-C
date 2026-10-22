/*
 * A350896  Number of partitions of n such that 4*(smallest part) = (number of
 *          parts).
 * https://oeis.org/A350896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{s >= 1, 4*s^2 <= n} P_{4*s-1}(n - 4*s^2) * where
 *            P_m(t) is the number of partitions of t into at most m parts
 * Range:     u8 n <= 32, u16 n <= 76, u32 n <= 215, u64 n <= 691,
 *            u128 n <= 2409, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 4)
        return 0; /* no valid partitions when n<4 */

    /* Find maximum s such that 4*s^2 <= n */
    idx_t s_max = 0;
    for (idx_t s = 1; 4 * s * s <= n; ++s)
        s_max = s;

    if (s_max == 0)
        return 0; /* safety */

    /* Parameters for the DP table. */
    idx_t K_needed = 4 * s_max - 1; /* maximum number of parts allowed   */
    idx_t N_needed = n;             /* maximum sum we need to handle     */

    /* Static DP table: P[k][m] stored row‑major, where row index k (0..K_needed)
     * and column index m (0..N_needed).  P[k][m] = number of partitions of m
     * into at most k parts. */
    static seq_t *ptab      = NULL; /* pointer to allocated table */
    static idx_t tables_n   = 0;    /* maximum n previously computed */
    static idx_t tables_k   = 0;    /* maximum k previously computed */

    /* If the required table is not yet built, build or enlarge it. */
    if (n > tables_n || K_needed > tables_k) {
        idx_t new_n   = N_needed;
        idx_t new_k   = K_needed;
        size_t stride = (size_t)new_n + 1;
        size_t size   = (size_t)(new_k + 1) * stride;

        /* Allocate the new table. The cast is required for C++ builds. */
        seq_t *newpt = (seq_t *)malloc(size * sizeof(seq_t));
        if (!newpt)
            return 0; /* allocation failure (unlikely) */

        /* Initialise to 0; set P[0][0] = 1. */
        memset(newpt, 0, size * sizeof(seq_t));
        newpt[0] = 1;

        /* Build partitions DP: P[k][m] = P[k-1][m] + P[k][m-k] (if m>=k). */
        for (idx_t k = 1; k <= new_k; ++k) {
            size_t base_k   = (size_t)k * stride;
            size_t base_km1 = (size_t)(k - 1) * stride;
            for (idx_t m = 0; m <= new_n; ++m) {
                seq_t val = newpt[base_km1 + m];
                if (m >= k)
                    val += newpt[base_k + (m - k)];
                newpt[base_k + m] = val;
            }
        }

        /* Replace old table. */
        free(ptab);
        ptab       = newpt;
        tables_n   = new_n;
        tables_k   = new_k;
    }

    /* Compute the result by summing over all valid s. */
    seq_t result = 0;
    size_t stride = (size_t)tables_n + 1;
    for (idx_t s = 1; 4 * s * s <= n; ++s) {
        idx_t R = n - 4 * s * s;      /* remaining sum after subtracting 4*s^2 */
        idx_t K = 4 * s - 1;           /* at most 4*s-1 parts          */
        result += ptab[(size_t)K * stride + R];
    }
    return result;
}
