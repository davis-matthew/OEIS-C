/*
 * A251225  Number of (n+1)X(5+1) 0..1 arrays with no 2X2 subblock having the
 *          minimum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A251225
   (Number of (n+1)×6 binary arrays with no forbidden 2×2 subblock.)
   The value is computed by dynamic programming over rows.
   For each successive row t, we add all previous rows s that together
   with t satisfy the requirement that for all 5 adjacent column pairs
   (j,j+1) the inequality
          min(a,d) > abs(b−c)
   does not hold (here a,b are bits in row s, c,d in row t):
   the forbidden configuration occurs only when a = d = 1 and b = c.
*/

#include <stdint.h>
#include <stdbool.h>

/* trans[s][t] == true iff a 2×2 subblock formed by rows s and t
   (s is the previous row, t is the new row) is allowed. */
static bool trans[64][64];
static bool trans_ready = false;

static void init_transpose(void)
{
    if (trans_ready) return;
    for (int s = 0; s < 64; ++s) {
        for (int t = 0; t < 64; ++t) {
            bool ok = true;
            for (int j = 0; j < 5; ++j) {
                unsigned a = (s >> j) & 1;
                unsigned b = (s >> (j + 1)) & 1;
                unsigned c = (t >> j) & 1;
                unsigned d = (t >> (j + 1)) & 1;
                /* Forbidden only when a=d=1 and b=c. */
                if (a == 1 && d == 1 && b == c) {
                    ok = false;
                    break;
                }
            }
            trans[s][t] = ok;
        }
    }
    trans_ready = true;
}

seq_t a(idx_t n)
{
    init_transpose();

    /* dp[s] -- number of ways to reach current last row state s.
       newdp[t] -- number of ways after adding a new row t. */
    seq_t dp[64], newdp[64];
    for (int i = 0; i < 64; ++i) dp[i] = (seq_t)1;  /* first row can be any of 64 patterns */

    /* We need n transitions (rows = n+1). */
    for (idx_t step = 0; step < n; ++step) {
        for (int t = 0; t < 64; ++t) {
            seq_t sum = 0;
            for (int s = 0; s < 64; ++s) {
                if (trans[s][t]) sum += dp[s];
            }
            newdp[t] = sum;
        }
        /* copy newdp -> dp for next iteration */
        for (int t = 0; t < 64; ++t) dp[t] = newdp[t];
    }

    /* total number of arrays is sum over final row states */
    seq_t total = 0;
    for (int t = 0; t < 64; ++t) total += dp[t];
    return total;
}
