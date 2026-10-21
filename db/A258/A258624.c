/*
 * A258624  Number of length n 1..(1+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5 or 7.
 * https://oeis.org/A258624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 32, u32 n <= 63, u64 n <= 128,
 *            u128 n <= 210, big any n
 * Helpers:   gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1  
   let g(r) be true iff gcd(r,210)>1.  
   dp[1][2]=1; all other dp[1][r]=0.  
   For each step k (1<=k<n) and residue r with g(r):
          if g((r+1)%210) then dp[k+1][(r+1)%210] += dp[k][r];
          if g((r+2)%210) then dp[k+1][(r+2)%210] += dp[k][r];
   then a(k+1) = sum_{r} dp[k+1][r] over all g(r).  */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd_int.h"

/* small gcd for int */

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* not defined for n=0, but keep safe */

    /* good residues modulo 210 */
    static bool good[210];
    static bool init = false;
    if (!init) {
        for (int r = 0; r < 210; ++r)
            good[r] = (gcd_int(r, 210) != 1);
        init = true;
    }

    if (n == 1) return 1;  /* only the sequence [2] */

    seq_t dp_cur[210] = {0};
    seq_t dp_next[210] = {0};

    dp_cur[2] = 1;  /* prefix sum after first element = 2 */

    for (idx_t len = 2; len <= n; ++len) {
        memset(dp_next, 0, 210 * sizeof(seq_t));

        for (int r = 0; r < 210; ++r) {
            if (!good[r]) continue;
            seq_t ways = dp_cur[r];
            if (!ways) continue;

            int r1 = (r + 1) % 210;
            if (good[r1]) dp_next[r1] += ways;

            int r2 = (r + 2) % 210;
            if (good[r2]) dp_next[r2] += ways;
        }

        if (len == n) {
            seq_t sum = 0;
            for (int r = 0; r < 210; ++r)
                if (good[r]) sum += dp_next[r];
            return sum;
        }

        /* prepare for next iteration */
        memcpy(dp_cur, dp_next, 210 * sizeof(seq_t));
    }

    return 0;  /* unreachable */
}
