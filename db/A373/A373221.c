/*
 * A373221  Expansion of Product_{i>=1, j>=0} (1 + x^(i * 7^j)).
 * https://oeis.org/A373221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 70, u32 n <= 210, u64 n <= 693,
 *            u128 n <= 2450, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function returns the n‑th coefficient of the generating function  
 * ∏_{i≥1, j≥0} (1 + x^{i · 7^j}).  
 * It implements a subset‑sum DP over all pairs (i, j) with weight ≤ n.  
 * The DP array `dp[k]` holds the number of ways to obtain the sum `k` using
 * any subset of the pairs considered so far.  For each pair we update the
 * DP backwards to avoid counting a pair more than once.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* The DP array stores values up to index n. */
    size_t N = (size_t)n;
    seq_t *dp = alloc_seq_array(N + 1);

    /* Initialise all entries to zero; dp[0] = 1 marks the empty subset. */
    for (size_t i = 0; i <= N; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Enumerate all pairs (i, j) with i·7^j ≤ n.  Each pair corresponds
     * to a distinct factor (1 + x^{i·7^j}) in the product. */
    idx_t pow = 1;                     /* current 7^j */
    while (pow <= n)
    {
        idx_t limit_i = n / pow;       /* largest i with i·pow ≤ n */

        for (idx_t i = 1; i <= limit_i; ++i)
        {
            idx_t weight = i * pow;    /* i · 7^j */

            /* Update DP backwards: for any sum s ≥ weight, add ways to form
             * s‑weight.  This is the standard subset‑sum update. */
            for (idx_t s = N; s >= weight; --s)
                dp[s] += dp[s - weight];
        }

        /* Prepare next power of 7, guarding against overflow. */
        if (pow > n / 7) break;
        pow *= 7;
    }

    seq_t result = dp[N];
    free_seq_array(dp);
    return result;
}
