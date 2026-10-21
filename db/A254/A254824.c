/*
 * A254824  Number of length n 1..(5+1) arrays with every leading partial sum
 *          divisible by 2, 3 or 5.
 * https://oeis.org/A254824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic programming over residues modulo 30 for arrays of
   length n with elements in {1,…,6}.  At every prefix the sum must be
   divisible by 2, 3, or 5.  The recurrence is:
     dp[0][0] = 1, dp[0][r] = 0 (r>0)
     dp[k+1][s] = Σ_{t allowed} Σ_{v=1..6} [s = (t+v) mod 30] * dp[k][t]
   where “allowed” means residue r%2==0 || r%3==0 || r%5==0.
   The answer a(n) = Σ_{s allowed} dp[n][s]. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    /* residues 0…29 that are multiples of 2 or 3 or 5 */
    static const bool allowed[30] = {
        true,  /* 0  → 0 mod 2,3,5 */
        false, /* 1  */
        true,  /* 2  → 0 mod 2 */
        true,  /* 3  → 0 mod 3 */
        true,  /* 4  → 0 mod 2 */
        true,  /* 5  → 0 mod 5 */
        true,  /* 6  → 0 mod 2,3 */
        false, /* 7  */
        true,  /* 8  → 0 mod 2 */
        true,  /* 9  → 0 mod 3 */
        true,  /*10  → 0 mod 2,5 */
        false, /*11  */
        true,  /*12  → 0 mod 2,3 */
        false, /*13  */
        true,  /*14  → 0 mod 2 */
        true,  /*15  → 0 mod 2,3,5 */
        true,  /*16  → 0 mod 2 */
        false, /*17  */
        true,  /*18  → 0 mod 2,3 */
        false, /*19  */
        true,  /*20  → 0 mod 2,5 */
        true,  /*21  → 0 mod 3 */
        true,  /*22  → 0 mod 2 */
        false, /*23  */
        true,  /*24  → 0 mod 2,3 */
        true,  /*25  → 0 mod 5 */
        true,  /*26  → 0 mod 2 */
        true,  /*27  → 0 mod 3 */
        true,  /*28  → 0 mod 2 */
        false   /*29  */
    };

    seq_t dp_prev[30] = {0};
    seq_t dp_next[30] = {0};

    dp_prev[0] = (seq_t)1;  /* empty prefix sum is 0, which is allowed */

    for (idx_t step = 1; step <= n; ++step) {
        /* reset next step array */
        for (int i = 0; i < 30; ++i)
            dp_next[i] = (seq_t)0;

        /* propagate counts */
        for (int t = 0; t < 30; ++t) {
            if (!allowed[t]) continue;
            seq_t val = dp_prev[t];
            if (val == (seq_t)0) continue;
            for (int v = 1; v <= 6; ++v) {
                int s = (t + v) % 30;
                if (!allowed[s]) continue;
                dp_next[s] += val;
            }
        }

        /* copy dp_next → dp_prev for next iteration */
        for (int i = 0; i < 30; ++i)
            dp_prev[i] = dp_next[i];
    }

    /* sum over all allowed residues after n steps */
    seq_t result = (seq_t)0;
    for (int s = 0; s < 30; ++s)
        if (allowed[s])
            result += dp_prev[s];

    return result;
}
