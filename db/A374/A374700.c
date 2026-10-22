/*
 * A374700  Triangle read by rows where T(n,k) is the number of integer
 *          compositions of n whose leaders of strictly increasing runs sum to
 *          k.
 * https://oeis.org/A374700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading triangle T(n,k) row‑wise, * where
 *            T(n,k) = number of integer compositions of n whose leaders * of
 *            strictly increasing runs sum to k
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_ROW 150          /* enough for n up to ~10000 */

static bool built = false;
static seq_t dp[MAX_ROW+1][MAX_ROW+1][MAX_ROW+1];
static seq_t tri[MAX_ROW+1][MAX_ROW+1];

/* convert linear index n to (row, col) in the triangle */
static inline void index_to_rowcol(idx_t n, idx_t *row, idx_t *col)
{
    idx_t r = 0;
    while ((r+1)*(r+2)/2 <= n) ++r;
    *row = r;
    *col = n - r*(r+1)/2;
}

/* build dp[sum][last][k] and triangle tri[n][k] once */
static void build_triangle(void)
{
    /* base case: single part composition */
    for (idx_t l = 1; l <= MAX_ROW; ++l)
        dp[l][l][l] = (seq_t)1;

    /* transition */
    for (idx_t sum = 1; sum < MAX_ROW; ++sum) {
        idx_t max_w = MAX_ROW - sum;
        for (idx_t last = 1; last <= sum; ++last) {
            for (idx_t k = 1; k <= sum; ++k) {
                seq_t val = dp[sum][last][k];
                if (!val) continue;
                for (idx_t w = 1; w <= max_w; ++w) {
                    idx_t newsum = sum + w;
                    idx_t newlast = w;
                    idx_t newk = k + ((w <= last) ? w : 0);
                    dp[newsum][newlast][newk] += val;
                }
            }
        }
    }

    /* extract triangle */
    tri[0][0] = (seq_t)1;
    for (idx_t n = 1; n <= MAX_ROW; ++n) {
        for (idx_t k = 0; k <= n; ++k) {
            seq_t sum = 0;
            for (idx_t last = 1; last <= n; ++last)
                sum += dp[n][last][k];
            tri[n][k] = sum;
        }
    }
    built = true;
}

seq_t a(idx_t n)
{
    if (!built) build_triangle();

    idx_t row, col;
    index_to_rowcol(n, &row, &col);

    if (row > MAX_ROW || col > row) return (seq_t)0;
    return tri[row][col];
}
