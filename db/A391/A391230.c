/*
 * A391230  Number of integer partitions of n > 0 such that the least and
 *          greatest parts are not both odd (equivalently, their product is
 *          even).
 * https://oeis.org/A391230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer partitions of n such that * at least
 *            one of the smallest or largest parts is even * (equivalently,
 *            the product of the smallest and largest parts is even)
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;            /* dp[sum*4 + state]  */
static idx_t max_comp = 0;          /* maximum n for which dp is built */

/* State encoding:  (min_parity << 1) | max_parity
 * 0 : min even, max even
 * 1 : min even, max odd
 * 2 : min odd,  max even
 * 3 : min odd,  max odd   <-- exclude for a(n) */
            
static void compute_dp(idx_t limit)
{
    size_t entries = (size_t)(limit + 1) * 4;
    seq_t *new_dp = (seq_t *)malloc(entries * sizeof(seq_t));
    if (!new_dp) return;  /* allocation failure – caller will handle */

    /* zero initialisation (safe for integer and arithmetic types) */
    for (size_t i = 0; i < entries; ++i) new_dp[i] = 0;

    /* standard unbounded‑knapsack DP counting partitions
     * while keeping parity of the smallest and largest parts. */
    for (idx_t t = 1; t <= limit; ++t) {
        int par = (int)(t & 1);        /* parity of current part */
        for (idx_t sum = t; sum <= limit; ++sum) {
            idx_t src = sum - t;      /* partition of the remaining sum */
            /* combine with existing partitions of sum-t */
            for (int s = 0; s < 4; ++s) {
                seq_t cnt = new_dp[(size_t)src * 4 + s];
                if (!cnt) continue;
                int minp = s >> 1;                     /* extract min parity */
                int new_state = (minp << 1) | par;      /* new max parity */
                new_dp[(size_t)sum * 4 + new_state] += cnt;
            }
            /* single part partition when sum == t */
            if (sum == t) {
                int idxnew = par * 3;  /* 0 if even, 3 if odd */
                new_dp[(size_t)sum * 4 + idxnew] += 1;
            }
        }
    }

    free(dp);
    dp = new_dp;
    max_comp = limit;
}

/* Public function as required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined by OEIS, but safe */
    if (n > max_comp || dp == NULL)       /* build or expand DP table */
        compute_dp(n);

    /* a(n) = total partitions – those with both smallest and largest odd */
    seq_t a0 = dp[(size_t)n * 4 + 0];
    seq_t a1 = dp[(size_t)n * 4 + 1];
    seq_t a2 = dp[(size_t)n * 4 + 2];
    /* state 3 (both odd) is excluded */
    return a0 + a1 + a2;
}
