/*
 * A257067  Number of length 5 1..(n+1) arrays with every leading partial sum
 *          divisible by 2 or 3.
 * https://oeis.org/A257067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count the number of length‑5 arrays with elements in 1..(n+1)
 *            such that every leading partial sum is divisible by 2 or 3. The
 *            computation uses a DP over the 6 residues modulo 6, keeping only
 *            residues 0,2,3,4 which are the admissible ones
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 126, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const int allowed[4] = {0, 2, 3, 4};

    /* Count how many of the numbers 1 .. (n+1) have each residue modulo 6. */
    seq_t M = (seq_t)n + 1;          /* M = n + 1, in seq_t to avoid overflow */
    seq_t freq[6];
    for (int s = 0; s < 6; ++s) {
        seq_t first = (s == 0 ? 6 : (seq_t)s); /* first number >=1 with residue s */
        if (first > M) {
            freq[s] = 0;
        } else {
            freq[s] = 1 + (M - first) / 6;
        }
    }

    /* DP: dp[r] = number of valid prefixes of current length whose
       cumulative sum ≡ r (mod 6).  Start with length 0, sum = 0. */
    seq_t dp[6] = {0};
    dp[0] = 1;

    for (int step = 0; step < 5; ++step) {
        seq_t newdp[6] = {0};

        for (int r = 0; r < 6; ++r) {
            if (dp[r] == 0) continue;

            for (int i = 0; i < 4; ++i) {        /* iterate over allowed residues */
                int t = allowed[i];
                int s = (t - r + 6) % 6;          /* residue that a new element must have */
                seq_t cnt = freq[s];
                if (cnt == 0) continue;

                newdp[t] += dp[r] * cnt;
            }
        }

        /* copy newdp to dp for next iteration */
        for (int r = 0; r < 6; ++r) dp[r] = newdp[r];
    }

    /* Sum over all allowed residues to get the final count. */
    seq_t ans = 0;
    for (int i = 0; i < 4; ++i) ans += dp[allowed[i]];
    return ans;
}
