/*
 * A243862  Number of length n sequences on alphabet {0,1,2} that contain all
 *          of 00, 01, 02, 10, 11, 12, 20, 21, 22 as (possibly overlapping)
 *          contiguous subsequences.
 * https://oeis.org/A243862
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 21, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes a(n): the number of length‑n sequences on the alphabet
   {0,1,2} that contain every 2‑letter word (00,01,02,10,11,12,20,21,22)  
   as a contiguous substring.  
   The algorithm counts walks of length n‑1 on the directed complete
   graph on 3 vertices that visit all 9 directed edges at least once.
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 10) return 0;          /* defined only for n ≥ 10 */

    const idx_t L = n - 1;         /* number of edges in a word of length n */

    /* dp[cur][v][mask] : number of walks of current length ending at
       vertex v that have covered the edges indicated by mask (bits 0..8) */
    static seq_t dp[2][3][512];

    int cur = 0, nxt = 1;

    /* initialize all counts to 0 */
    for (int v = 0; v < 3; ++v)
        for (int m = 0; m < 512; ++m)
            dp[0][v][m] = dp[1][v][m] = 0;

    /* start from each of the 3 vertices, with no edges seen yet */
    for (int v = 0; v < 3; ++v)
        dp[cur][v][0] = 1;

    /* iterate over the L edges of the word */
    for (idx_t step = 0; step < L; ++step) {
        /* zero the next layer */
        for (int v = 0; v < 3; ++v)
            for (int m = 0; m < 512; ++m)
                dp[nxt][v][m] = 0;

        /* transition from cur to nxt */
        for (int v = 0; v < 3; ++v) {
            for (int m = 0; m < 512; ++m) {
                seq_t ways = dp[cur][v][m];
                if (!ways) continue;
                for (int v2 = 0; v2 < 3; ++v2) {
                    int e = v * 3 + v2;          /* edge index 0..8 */
                    int m2 = m | (1 << e);
                    dp[nxt][v2][m2] += ways;
                }
            }
        }
        /* swap layers */
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* sum over all ending vertices, require all 9 edges have been seen */
    seq_t ans = 0;
    for (int v = 0; v < 3; ++v)
        ans += dp[cur][v][511];
    return ans;
}
