/*
 * A254822  Number of length n 1..(3+1) arrays with every leading partial sum
 *          divisible by 2, 3 or 5.
 * https://oeis.org/A254822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A254822: a(n) = number of length n arrays of {1,2,3,4} for which every
   prefix sum is divisible by 2, 3, or 5.  Computed by dynamic programming
   on residues modulo LCM(2,3,5)=30. */

static const unsigned char ALLOWED[30] = {
    1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* sequence defined for n ≥ 1 */

    seq_t dp[30] = {0};
    seq_t newdp[30];
    dp[0] = (seq_t)1;                       /* empty sum (0) is allowed */

    for (idx_t step = 0; step < n; ++step) {
        /* clear newdp */
        for (int r = 0; r < 30; ++r) newdp[r] = 0;

        for (int r = 0; r < 30; ++r) {
            seq_t val = dp[r];
            if (!val) continue;
            if (!ALLOWED[r]) continue;      /* only transitions from allowed states */

            /* append digits 1..4 */
            for (int a = 1; a <= 4; ++a) {
                int nr = r + a;
                if (nr >= 30) nr -= 30;
                if (ALLOWED[nr]) newdp[nr] += val;
            }
        }

        /* move newdp to dp for next step */
        for (int r = 0; r < 30; ++r) dp[r] = newdp[r];
    }

    /* all stored states are already allowed, sum them all */
    seq_t ans = 0;
    for (int r = 0; r < 30; ++r) ans += dp[r];
    return ans;
}
