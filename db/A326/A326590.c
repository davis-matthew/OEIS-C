/*
 * A326590  Sum of the ninth largest parts of the partitions of n into 10
 *          parts.
 * https://oeis.org/A326590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS seq A326590: Sum of the ninth largest parts of the partitions of n into 10 parts */

#include <stdlib.h>
#include <stddef.h>

/*  Pre‑compute the number of partitions of any integer x into at most 8 parts.
 *  For a given n, the required sum is
 *        a(n) = Σ_{v=1}^{⌊(n-1)/9⌋} Σ_{a1=1}^{min(v, n-9v)} v · p8(n-9v-a1)
 *  where p8(s) is the number of partitions of s into at most 8 parts. */
seq_t a(idx_t n)
{
    /* n must be at least 10 to have 10 positive parts; otherwise the sum is 0. */
    if (n < 10) return 0;

    /* dynamic programming table for partitions into at most 8 parts */
    static seq_t *dp = NULL;
    static idx_t dp_cap = 0;                /* dp_cap is the current size of dp */

    /* We need dp values up to n (inclusive) for safety. */
    idx_t need = n;                         /* calculate dp[0..need] */
    if (dp == NULL || need + 1 > dp_cap) {
        idx_t new_cap = need + 1;
        /* round up to a power of two multiple for fewer reallocs */
        new_cap = (new_cap + 1023) & ~((idx_t)1023);
        dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        dp_cap = new_cap;
        /* initialize table */
        for (size_t i = 0; i < new_cap; ++i) dp[i] = 0;
        dp[0] = 1;
        /* unlimited coin change with coin values 1..8 gives partitions into <=8 parts */
        for (idx_t coin = 1; coin <= 8; ++coin) {
            for (size_t j = coin; j < new_cap; ++j) {
                dp[j] += dp[j - coin];
            }
        }
    }

    seq_t total = 0;
    idx_t maxV = (n - 1) / 9;               /* floor((n-1)/9) */

    for (idx_t v = 1; v <= maxV; ++v) {
        idx_t s = n - 9 * v;               /* remaining sum after removing a2 and a3..a10 */
        idx_t max_a1 = s < v ? s : v;      /* a1 can be at most v and at most s */

        for (idx_t a1 = 1; a1 <= max_a1; ++a1) {
            idx_t S = s - a1;              /* sum for the 8 parts after subtracting v */
            seq_t cnt = dp[S];             /* p8(S) */
            if (cnt) total += cnt * (seq_t)v;
        }
    }

    return total;
}
