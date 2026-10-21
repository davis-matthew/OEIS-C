/*
 * A254833  Number of length 6 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3 or 5.
 * https://oeis.org/A254833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length-6 arrays over 1..(n+1) whose every
 *            leading partial sum is divisible by 2 or 3 or 5
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 54, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Do not modify the following types; they are supplied by the build environment. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* residues modulo 30 that are acceptable: multiple of 2, 3 or 5 */
    static const bool allowed[30] = {
        1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0
    };

    seq_t freq[30];
    seq_t n1 = (seq_t)n + 1;                     /* (n+1) as seq_t  */
    for (idx_t r = 0; r < 30; ++r) {
        seq_t add = (seq_t)((30 - r) % 30);     /* add = (30 - r) mod 30 */
        freq[r] = (n1 + add) / 30;              /* number of integers in 1..(n+1) with residue r */
    }

    seq_t dp[30]  = {0};
    seq_t newdp[30];
    dp[0] = 1;                                  /* initial sum 0 */

    for (int step = 0; step < 6; ++step) {       /* build length-6 arrays */
        for (int i = 0; i < 30; ++i) newdp[i] = 0;

        for (int s = 0; s < 30; ++s) {
            if (dp[s] == 0) continue;
            for (int t = 0; t < 30; ++t) {
                if (freq[t] == 0) continue;
                int r = (s + t) % 30;
                if (!allowed[r]) continue;
                newdp[r] += dp[s] * freq[t];
            }
        }

        for (int i = 0; i < 30; ++i) dp[i] = newdp[i];
    }

    seq_t total = 0;
    for (int i = 0; i < 30; ++i) total += dp[i];
    return total;
}
