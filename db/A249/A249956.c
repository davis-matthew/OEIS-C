/*
 * A249956  Number of length n+5 0..4 arrays with no six consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A249956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+5 arrays over {0,1,2,3,4} such that in
 *            every block of six consecutive terms there is no pair of terms
 *            whose maximum equals the minimum of the remaining four terms
 * Range:     u8 none, u16 n <= 2, u32 n <= 11, u64 n <= 28, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    #include <stdint.h>
    #include <stdbool.h>

    /* There are 5^5 = 3125 possible 5‑digit suffixes and 5^6 = 15625
       possible 6‑digit windows.  We precompute for each 6‑digit window
       whether it is valid (no pair violates the condition). */
    static unsigned char valid[15625];
    static bool init = false;
    static seq_t dp[2][3125];
    const int POW5_4 = 625;   /* 5^4 */

    if (!init) {
        for (int idx = 0; idx < 15625; ++idx) {
            int tmp = idx;
            int d[6];
            /* decode the 6 digits of this window */
            for (int i = 5; i >= 0; --i) { d[i] = tmp % 5; tmp /= 5; }
            bool ok = true;
            for (int i = 0; i < 6 && ok; ++i) {
                for (int j = i + 1; j < 6 && ok; ++j) {
                    int mx = d[i] > d[j] ? d[i] : d[j];
                    int mn = 5;
                    for (int r = 0; r < 6; ++r)
                        if (r != i && r != j && d[r] < mn) mn = d[r];
                    if (mx == mn) ok = false;
                }
            }
            valid[idx] = ok;
        }
        init = true;
    }

    /* For sequences of length 5 there is exactly one way for each
       possible 5‑digit suffix. */
    for (int s = 0; s < 3125; ++s) dp[0][s] = (seq_t)1;
    int cur = 0, nxt = 1;

    /* We need to add n more digits to reach length n+5 (so n transitions). */
    for (uint64_t step = 0; step < n; ++step) {
        /* clear next layer */
        for (int s = 0; s < 3125; ++s) dp[nxt][s] = (seq_t)0;

        for (int s = 0; s < 3125; ++s) {
            seq_t val = dp[cur][s];
            if (val == 0) continue;

            int base6 = s * 5;               /* encode window = s*5 + d */
            int rem   = s % POW5_4;           /* drop oldest digit */

            for (int d = 0; d < 5; ++d) {
                if (!valid[base6 + d]) continue;

                int new_s = rem * 5 + d;      /* new 5‑digit suffix */
                dp[nxt][new_s] += val;
            }
        }
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* Sum all counts for the final 5‑digit suffixes. */
    seq_t total = (seq_t)0;
    for (int s = 0; s < 3125; ++s) total += dp[cur][s];
    return total;
}
