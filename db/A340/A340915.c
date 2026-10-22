/*
 * A340915  Number of ways to write n as an ordered sum of 8 squares of
 *          positive integers.
 * https://oeis.org/A340915
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as an ordered sum of 8 squares
 *            of positive integers
 * Range:     u8 n <= 24, u16 n <= 128, u32 n <= 4031, u64 n <= 9979,
 *            u128 n <= 9979, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* values below the offset (8) are irrelevant for the tests */
    if (n < 8)
        return 0;

    /* recompute if the cache is empty or must be enlarged */
    if (cache == NULL || n >= cache_size)
    {
        idx_t newlen = n + 1;
        /* pre‑compute at least up to 10000 to cover the usual test range */
        if (newlen < 10001)
            newlen = 10001;

        size_t cap = (size_t)newlen;

        seq_t *dp = (seq_t *)malloc(cap * sizeof(seq_t));
        if (dp == NULL) return 0;

        /* initialise with 1 way to form 0 using 0 squares */
        for (idx_t i = 0; i < newlen; ++i) dp[i] = 0;
        dp[0] = 1;

        /* convolve 8 times with the series Σ x^{k^2}, k≥1 */
        for (idx_t step = 0; step < 8; ++step)
        {
            seq_t *next = (seq_t *)calloc(cap, sizeof(seq_t));
            if (next == NULL) return 0;

            for (idx_t i = 0; i < newlen; ++i)
            {
                seq_t val = dp[i];
                if (val == 0) continue;

                for (idx_t k = 1;; ++k)
                {
                    idx_t sq = k * k;
                    idx_t j = i + sq;
                    if (j >= newlen) break;
                    next[j] += val;
                }
            }
            free(dp);
            dp = next;
        }

        /* replace old cache */
        if (cache) free(cache);
        cache = dp;
        cache_size = newlen;
    }

    return cache[n];
}
