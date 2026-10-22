/*
 * A345551  Numbers that are the sum of ten cubes in three or more ways.
 * https://oeis.org/A345551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A345551: numbers that are the sum of ten cubes in three or more ways.
   Implementation: a bounded dynamic‑programming search that counts, for each
   sum, how many unordered representations with exactly ten cubes exist,
   capping the count at 3.  All qualifying sums are stored in a statically
   allocated cache which is extended as needed. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* array of terms */
static size_t cache_len = 0;    /* number of terms currently cached */

/* Extend the cache so that at least 'need' terms are available. */
static void rebuild_cache(idx_t need)
{
    if (need <= cache_len)
        return; /* already have enough */

    /* Start with a bound that grows with the current cache size. */
    idx_t bound = cache_len > 100 ? (idx_t)(cache_len * 5) : 500;

    while (1) {
        /* Ensure bounding value fits in size_t. */
        if (bound > (idx_t)SIZE_MAX - 1000)
            bound = (idx_t)SIZE_MAX - 1000;

        size_t B = (size_t)bound;           /* maximum sum to consider */
        size_t rows = 11;                   /* k = 0…10 */
        size_t dp_size = (B + 1) * rows;    /* number of dp cells */

        unsigned char *dp = (unsigned char *)malloc(dp_size);
        if (!dp)
            abort();                      /* out of memory */

        /* Zero‑initialize and seed dp[0][0] = 1. */
        for (size_t i = 0; i < dp_size; ++i)
            dp[i] = 0;
        dp[0] = 1;                         /* dp[0* (B+1) + 0] */

        /* Enumerate cube values. */
        for (idx_t v = 1; ; ++v) {
            idx_t cube = v * v * v;
            if (cube > bound)
                break;                     /* no larger cubes useful */
            size_t cv = (size_t)cube;

            for (int k = 1; k <= 10; ++k) {
                size_t k_offset   = k   * (B + 1);
                size_t km1_offset = (k-1) * (B + 1);
                for (size_t sum = cv; sum <= B; ++sum) {
                    unsigned char add = dp[km1_offset + sum - cv];
                    if (add) {
                        unsigned int total = dp[k_offset + sum] + add;
                        dp[k_offset + sum] = (unsigned char)(total > 3 ? 3 : total);
                    }
                }
            }
        }

        /* Count how many sums have at least three representations. */
        size_t count = 0;
        for (size_t sum = 0; sum <= B; ++sum) {
            if (dp[10 * (B + 1) + sum] >= 3)
                count++;
        }

        if (count >= need) {
            /* Gather the qualifying sums into a new cache. */
            seq_t *new_cache = (seq_t *)malloc(count * sizeof(seq_t));
            if (!new_cache)
                abort();
            size_t pos = 0;
            for (size_t sum = 0; sum <= B; ++sum) {
                if (dp[10 * (B + 1) + sum] >= 3) {
                    new_cache[pos++] = (seq_t)sum;
                }
            }

            free(cache);
            cache = new_cache;
            cache_len = count;
            free(dp);
            return;
        }

        /* Not enough terms – enlarge the bound and retry. */
        free(dp);
        if (bound >= SIZE_MAX / 2)
            bound = SIZE_MAX / 2;
        else
            bound *= 2;
    }
}

/* Public sequence generator. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for 0 or negative indices */

    if (cache_len < (size_t)n)
        rebuild_cache(n);

    return cache[n - 1];
}
