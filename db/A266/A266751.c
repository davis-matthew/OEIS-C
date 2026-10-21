/*
 * A266751  G.f.:
 *          1/((1-t^11)^2*(1-t)*(1-t^3)*(1-t^5)*(1-t^7)*(1-t^9)*(1-t^13)*(1-t^15)*(1-t^17)*(1-t^19)*(1-t^21)).
 * https://oeis.org/A266751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A266751: Number of partitions of n using parts
   1, 3, 5, 7, 9, 11, 11, 13, 15, 17, 19, 21.
   Generating function:
   1 / ((1‑t^11)^2 * (1‑t) * (1‑t^3) * (1‑t^5) * (1‑t^7) * (1‑t^9)
          * (1‑t^13) * (1‑t^15) * (1‑t^17) * (1‑t^19) * (1‑t^21)) */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;          /* DP array, grows as needed */
static idx_t max_prepared = 0;       /* largest n computed so far */

/* Helper: compute dp[0..max] using coin change */
static void compute_dp(seq_t *dp, idx_t max)
{
    /* zero initialise */
    for (idx_t i = 0; i <= max; ++i) dp[i] = 0;
    dp[0] = 1;                         /* empty partition */

    /* part sizes: 1,3,5,7,9,11,11,13,15,17,19,21 */
    const int coins[] = {1,3,5,7,9,11,11,13,15,17,19,21};
    const unsigned int num_coins = sizeof(coins) / sizeof(coins[0]);

    for (unsigned int c = 0; c < num_coins; ++c) {
        idx_t coin = (idx_t)coins[c];
        for (idx_t i = coin; i <= max; ++i)
            dp[i] += dp[i - coin];
    }
}

seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if (cache == NULL || n > max_prepared) {
        idx_t new_max = n;
        seq_t *new_cache = alloc_seq_array(new_max + 1);
        if (!new_cache) abort();      /* allocation failure */

        compute_dp(new_cache, new_max);

        /* keep old cache alive to avoid invalid free */
        if (cache) seq_free_array(cache);
        cache = new_cache;
        max_prepared = new_max;
    }

    return cache[n];
}
