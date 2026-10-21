/*
 * A254831  Number of length 4 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3 or 5.
 * https://oeis.org/A254831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑4 arrays over 1..n+1 * whose every
 *            leading partial sum is * divisible by 2, 3 or 5. (OEIS A254831)
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Residues modulo 30 that are 0 mod 2, 3, or 5.  These are the only
   partial‑sum remainders allowed at each step of the array. */
static const bool VALID[30] = {
    1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Largest value that can appear in the array. */
    idx_t L = n + 1;            /* n fits in 64 bits for all test cases */

    seq_t dp[30]   = {0};       /* counts after the current number of steps */
    seq_t next[30] = {0};       /* counts for the next step */

    dp[0] = 1;   /* empty sum has residue 0 (valid) */

    /* Perform four transitions, one for each array element. */
    for (int step = 0; step < 4; ++step)
    {
        for (int i = 0; i < 30; ++i) next[i] = 0;

        for (int s = 0; s < 30; ++s) if (dp[s] != 0)
        {
            for (int t = 0; t < 30; ++t) if (VALID[t])
            {
                int d = t - s;
                if (d < 0) d += 30;          /* t ≡ s + a (mod 30) */

                idx_t cnt = 0;               /* numbers a∈[1,L] with a≡d (mod 30) */
                if (d == 0)
                    cnt = L / 30;            /* multiples of 30 */
                else if ((idx_t)d <= L)
                    cnt = (L - d) / 30 + 1;  /* first a≡d, then +30k */

                if (cnt == 0) continue;

                seq_t prod = dp[s] * (seq_t)cnt;  /* multiply by small integer */
                next[t] = next[t] + prod;         /* accumulate */
            }
        }
        for (int i = 0; i < 30; ++i) dp[i] = next[i];
    }

    /* Sum all reachable final residues (they are all allowed). */
    seq_t total = 0;
    for (int i = 0; i < 30; ++i)
        total = total + dp[i];
    return total;
}
