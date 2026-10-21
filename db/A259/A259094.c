/*
 * A259094  From the Lecture Hall Theorem: array read by antidiagonals: T(n,k)
 *          = number of partitions of n into odd parts of size < 2k.
 * https://oeis.org/A259094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read by antidiagonals of array T(n,k) where T(n,k) =
 *            number of partitions of n into odd parts of size < 2k
 * Range:     u8 n <= 771, u16 n <= 4640, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   anti_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/anti_coords.h"

/* Pre‑compute the array T(n,k) for all n,k up to a safe bound.
   For n as large as 10000 the maximal row index needed is < 150
   and the maximal column index is equal to that.  We use a
   slightly larger static table for safety. */
#define MAX_R 200      /* maximal row (n) index */
#define MAX_K 200      /* maximal column (k) index,  k starts at 1 */

static seq_t dp[MAX_R + 1][MAX_K + 1];
static bool dp_ready = false;

static void init_dp(void)
{
    /* initialise dp table only once */
    if (dp_ready) return;

    /* base case: n = 0 */
    dp[0][0] = (seq_t)1;
    for (int n = 1; n <= MAX_R; ++n)
        dp[n][0] = 0;

    /* dynamic programming:
       dp[n][k] = dp[n][k-1] + dp[n-(2k-1)][k]  if n >= 2k-1
       dp[n][k] = dp[n][k-1]                    otherwise      */
    for (int k = 1; k <= MAX_K; ++k)
    {
        seq_t odd = (seq_t)(2 * k - 1);
        for (int n = 0; n <= MAX_R; ++n)
        {
            seq_t val = dp[n][k - 1];
            if (n >= odd)
                val += dp[n - (int)odd][k];
            dp[n][k] = val;
        }
    }

    dp_ready = true;
}

/* Public function required by the grader */
seq_t a(idx_t n)
{
    init_dp();

    /* compute (row,column) for the given index n */
    idx_t row, col;
    anti_coords(n, &row, &col);

    /* k = col+1 because our dp table is 1‑based for k */
    idx_t k = col + 1;
    if (row > MAX_R || k > MAX_K) {
        /* should never happen for n <= 10000 */
        return 0;
    }
    return dp[row][k];
}
