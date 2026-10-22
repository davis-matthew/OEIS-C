/*
 * A357456  Number of partitions of n into two or more odd parts.
 * https://oeis.org/A357456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into odd parts minus 1 if n is
 *            odd (to exclude the one‑part partition)
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A357456 – Number of partitions of n into two or more odd parts. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for the special offset n = 0. */
    if (n == 0)
        return 0;

    /* Static cache of already computed partition numbers into odd parts.
       The array holds values dp[0..dp_max] where dp[i] = number of partitions of i
       into odd parts (including the empty partition for i = 0). */
    static seq_t *dp = NULL;
    static idx_t dp_max = 0;
    const idx_t PREFETCH_MAX = 10000;   /* pre‑compute up to 10 000 once */

    /* If the requested n is larger than what we have, recompute the cache. */
    if (dp == NULL || n > dp_max) {
        idx_t new_max = (n > PREFETCH_MAX) ? n : PREFETCH_MAX;

        /* Allocate and initialise a new array. */
        seq_t *newdp = (seq_t *)calloc(new_max + 1, sizeof(seq_t));
        if (!newdp)
            return 0; /* allocation failure – should not happen in normal testing */

        newdp[0] = 1;   /* empty partition */

        /* Coin change DP over odd part sizes. */
        for (idx_t k = 1; k <= new_max; k += 2) {
            for (idx_t s = k; s <= new_max; ++s)
                newdp[s] += newdp[s - k];
        }

        /* Replace old cache. */
        free(dp);
        dp = newdp;
        dp_max = new_max;
    }

    seq_t result = dp[n];
    if (n & 1)           /* subtract the one‑part partition for odd n */
        result -= 1;
    return result;
}
