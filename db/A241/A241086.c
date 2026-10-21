/*
 * A241086  Number of partitions p of n into distinct parts such that max(p)
 *          <= 2*(number of parts of p).
 * https://oeis.org/A241086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A241086
   a(n) = number of partitions of n into distinct parts such that
          max(part) <= 2 * (number of parts).
   The implementation enumerates all possible numbers of parts k,
   then counts the k‑element subsets of {1,…,2k} whose sum equals n.
   The algorithm runs in O(n √n) time and O(n √n) memory for a given n. */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 0u) {
        return 0;
    }

    /* Result array a[0 … n] */
    seq_t *a_arr = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!a_arr) {
        /* Allocation failed – return 0 to avoid crashes */
        return 0;
    }

    /* Upper bound on the number of parts: k(k+1)/2 <= n  ⇒  k <= (sqrt(8n+1)-1)/2 */
    idx_t kmax = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);

    /* Allocate DP table: (kmax+1) rows, (n+1) columns */
    size_t rows = (size_t)kmax + 1;
    size_t cols = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(rows * cols, sizeof(seq_t));
    if (!dp) {
        free(a_arr);
        return 0;
    }

    /* Main loop over the number of parts k */
    for (idx_t k = 1; ; ++k) {
        /* Minimum possible sum with k distinct positive parts */
        idx_t min_sum = k * (k + 1) / 2;
        if (min_sum > n) {
            break;          /* No larger k can contribute */
        }

        /* Reset DP rows 0 … k to zero */
        for (size_t j = 0; j <= (size_t)k; ++j) {
            seq_t *row = dp + j * cols;
            for (size_t s = 0; s < cols; ++s) {
                row[s] = 0;
            }
        }

        /* dp[0][0] = 1: empty subset */
        dp[0] = 1;

        idx_t limit = 2 * k;                     /* use numbers 1 … 2k */
        for (idx_t v = 1; v <= limit; ++v) {
            idx_t jmax = v < k ? v : k;
            for (idx_t j = jmax; j >= 1; --j) {
                seq_t *rowj   = dp + j * cols;
                seq_t *rowjm1 = dp + (j - 1) * cols;
                for (size_t s = cols - 1; s >= (size_t)v; --s) {
                    rowj[s] += rowjm1[s - (size_t)v];
                }
            }
        }

        /* Add contributions of partitions with exactly k parts */
        seq_t *rowk = dp + k * cols;
        for (size_t s = 0; s <= (size_t)n; ++s) {
            a_arr[s] += rowk[s];
        }
    }

    seq_t result = a_arr[(size_t)n];
    free(a_arr);
    free(dp);
    return result;
}
