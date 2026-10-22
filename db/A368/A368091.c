/*
 * A368091  Triangle read by rows. T(n, k) = Sum_{p in P(n, k)} Product_{r in
 *          p} r, where P(n, k) are the partitions of n with length k.
 * https://oeis.org/A368091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 258, u32 n <= 1092, u64 n <= 5276,
 *            u128 n <= 9747, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368091: triangle read by rows.
   T(n,0) = 1 if n==0 else 0.
   For k>0:
     T(n,k) = Σ_{x=1}^{⌊n/k⌋} x * P_{x}(n-x,k-1),
   where P_{x}(m,ℓ) denotes T(m,ℓ) computed with the restriction that
   all parts are ≥ x.  This restriction is implemented by a third
   dimension L (the minimal allowed part).  The DP table dp[n][k][L]
   holds the weighted sum for partitions of n into k parts with each part
   ≥ L.  Then T(n,k)=dp[n][k][1].  a(n) is obtained by reading the
   triangle row-wise. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_R 150                     /* sufficient for n≈10000          */

static seq_t dp[MAX_R + 1][MAX_R + 1][MAX_R + 2] = {0};
static idx_t cached_row_max = 0;
static bool dp_inited = false;

static void init_dp(void)
{
    if (dp_inited) return;
    dp_inited = true;
    for (idx_t L = 1; L <= MAX_R; ++L) {
        dp[0][0][L] = (seq_t)1;       /* empty partition has product 1   */
    }
}

seq_t a(idx_t n)
{
    /* locate (row, column) corresponding to index n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;      /* triangular numbers   */
    idx_t c = n - (r * (r + 1)) / 2;              /* column within row   */

    /* ensure DP table covers rows up to r */
    init_dp();
    if (cached_row_max < r) {
        for (idx_t nn = cached_row_max + 1; nn <= r; ++nn) {
            for (idx_t k = 1; k <= nn; ++k) {
                idx_t max_part = nn / k;          /* largest part allowed */
                for (idx_t L = nn; L >= 1; --L) {
                    seq_t sum = 0;
                    if (L <= max_part) {
                        for (idx_t x = L; x <= max_part; ++x) {
                            sum += (seq_t)x * dp[nn - x][k - 1][x];
                        }
                    }
                    dp[nn][k][L] = sum;
                }
            }
        }
        cached_row_max = r;
    }

    /* T(r,c) is stored as dp[r][c][1] (minimal part 1) */
    return dp[r][c][1];
}
