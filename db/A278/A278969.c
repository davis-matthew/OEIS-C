/*
 * A278969  Number of 7-digit numbers whose sum of digits is n.
 * https://oeis.org/A278969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A278969: Number of 7‑digit numbers whose sum of digits is n.
   Formula (generating function):  (x - x¹⁰)/(1 - x) * ((1 - x¹⁰)/(1 - x))⁶
   Computation: dynamic programming counting digit sums
   (first digit ∈ {1,…,9}, remaining digits ∈ {0,…,9}).  Only indices
   1 ≤ n ≤ 63 are non‑zero, others return 0.  The table is built once
   and reused, so subsequent calls are O(1). */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* table[0] is unused; indices 1..63 hold the values. */
    static seq_t table[64];
    static int built = 0;

    if (!built)
    {
        /* dp[pos][sum] = number of ways to choose first pos digits
           (with first digit non‑zero) having total sum = sum. */
        static seq_t dp[8][64];
        for (int i = 0; i < 8; ++i)
            for (int s = 0; s < 64; ++s)
                dp[i][s] = 0;

        dp[0][0] = 1;                      /* zero digits sum to 0 */

        for (int pos = 1; pos <= 7; ++pos)
        {
            int low = (pos == 1) ? 1 : 0;  /* first digit cannot be zero */
            for (int s = 0; s < 64; ++s)
            {
                seq_t before = dp[pos - 1][s];
                if (!before) continue;
                for (int d = low; d <= 9; ++d)
                {
                    int ns = s + d;
                    if (ns < 64)
                        dp[pos][ns] += before;
                }
            }
        }

        for (int sum = 1; sum < 64; ++sum)
            table[sum] = dp[7][sum];

        built = 1;
    }

    if (n >= 1 && n < 64)
        return table[(size_t)n];
    return 0;
}
