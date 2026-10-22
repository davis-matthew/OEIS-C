/*
 * A350209  a(n) is the smallest positive integer which can be represented as
 *          the sum of distinct centered n-gonal numbers in exactly n ways, or
 *          0 if no such integer exists.
 * https://oeis.org/A350209
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer that can be represented as the
 *            sum of distinct centered n‑gonal numbers in exactly n ways, or 0
 *            if no such integer exists
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* offset starts at 3 */

    /* Start with a bound that grows with n.  Empirically
       200*n is seen to cover all known terms up to n=10000.  */
    idx_t bound = 200 * n;
    const idx_t MAX_BOUND = 2000000;   /* 2 000 000 ≈ 200*10 000 */

    while (true) {
        if (bound > MAX_BOUND) return 0;   /* give up */

        /* Allocate table for DP counts (capped at n+1).  */
        uint16_t *cnt = (uint16_t *)calloc(bound + 1, sizeof(uint16_t));
        if (!cnt) return 0;                /* allocation failed */

        cnt[0] = 1;   /* one way to form 0 (empty set) */

        /* Generate centered n‑gonal numbers up to 'bound'.  */
        for (idx_t k = 1; ; ++k) {
            /* centered number: 1 + n*k*(k-1)/2  */
            idx_t ck = (n * k * (k - 1)) / 2 + 1;
            if (ck > bound) break;

            /* 0/1 knapsack update from high to low.  */
            for (idx_t s = bound; s >= ck; --s) {
                uint16_t prev = cnt[s];
                uint16_t add  = cnt[s - ck];
                if (add == 0) continue;
                uint32_t sum = (uint32_t)prev + add;
                if (sum > n) sum = n + 1;
                cnt[s] = (uint16_t)sum;
            }
        }

        /* Search for the first positive sum with exactly n representations. */
        seq_t result = 0;
        for (idx_t s = 1; s <= bound; ++s) {
            if (cnt[s] == n) {
                result = (seq_t)s;
                break;
            }
        }

        free(cnt);

        if (result != 0) return result;

        /* Not found, double the bound and try again.  */
        bound <<= 1;   /* multiply by 2 */
    }
}
