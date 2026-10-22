/*
 * A325194  Regular triangle read by rows where T(n,k) is the number of
 *          integer partitions of n with co-rank n - k, where co-rank is the
 *          greater of the length and the largest part.
 * https://oeis.org/A325194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A325194: number of integer partitions of k with co-rank k-(n-k)
   where T(n,k) = number of partitions of k with co-rank n-k.  The sequence
   a(n) is obtained by reading the triangle T row‑by‑row starting at a(0).  */
#include <stdint.h>
#include <stdbool.h>

/* The maximum row index needed for n up to about 10000 is 141.
   We fix a slightly larger bound for safety. */
#define MAX 150

static seq_t dp[MAX + 1][MAX + 1][MAX + 1];
static bool dp_initialized = false;

/* Precompute dp[i][j][l] = number of partitions of i using parts
   \le j and at most l parts.  The computation fills the 3‑D table
   with a simple recurrence. */
static void init_dp(void) {
    if (dp_initialized) return;
    for (idx_t i = 0; i <= MAX; ++i) {
        for (idx_t j = 0; j <= MAX; ++j) {
            for (idx_t l = 0; l <= MAX; ++l) {
                if (i == 0) {
                    dp[i][j][l] = (seq_t)1;
                } else if (j == 0 || l == 0) {
                    dp[i][j][l] = (seq_t)0;
                } else {
                    seq_t val = dp[i][j - 1][l];
                    if (i >= j) val += dp[i - j][j][l - 1];
                    dp[i][j][l] = val;
                }
            }
        }
    }
    dp_initialized = true;
}

/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static idx_t row_of_idx(idx_t n) {
    idx_t r = 0;
    while (true) {
        idx_t next = r + 1;
        __int128 tri = (__int128)next * (next + 1) / 2;
        if (tri > n) break;
        r = next;
    }
    return r;
}

seq_t a(idx_t n) {
    init_dp();

    /* Determine the row (n) and column (k) corresponding to the
       triangular index n.  The co‑rank m is n - k. */
    idx_t r = row_of_idx(n);
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;          /* column index within the row */
    idx_t m = r - k;              /* co‑rank value n - k */

    /* S(m,k) = number of partitions of k that fit inside an m × m
       square (both largest part ≤ m and number of parts ≤ m). */
    seq_t Sm  = m <= MAX ? dp[k][m][m] : (seq_t)0;
    seq_t Sm1 = (m > 0 && m - 1 <= MAX) ? dp[k][m - 1][m - 1] : (seq_t)0;

    return Sm - Sm1;
}
