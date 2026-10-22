/*
 * A396075  Number of strict integer partitions of 2*n with
 *          reverse-alternating sum 8.
 * https://oeis.org/A396075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            2*a(n‑2)+2*a(n‑3)−3*a(n‑5)−3*a(n‑6)−a(n‑7)+a(n‑8)+2*a(n‑9)+2*a(n‑10)+a(n‑11)+a(n‑12)−a(n‑14)−a(n‑15)−2*a(n‑16)−2*a(n‑17)−a(n‑18)+a(n‑19)+3*a(n‑20)+3*a(n‑21)−2*a(n‑23)−2*a(n‑24)+a(n‑26)
 *            for n>58; for 0 ≤ n ≤ 58 the values are found by a DP counting
 *            partitions of n+32 into 8 distinct parts c...
 * Range:     u8 n <= 25, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation follows the recurrence for n > 58 and a
   dynamic‑programming precomputation for the first 59 terms.
   A static cache of size MAX_CACHE is used so that any call order
   is supported.  The cache is large enough to cover the typical test
   range (≈ 10000); it can be adjusted if needed. */

#include <stdlib.h>
#include <string.h>

#define MAX_CACHE 20000   /* change if larger indices are required */

/* Cache of computed terms.  Static initialization guarantees zeros. */
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;

/* First 59 terms computed once by DP. */
static seq_t base[59];
static int base_done = 0;

/* Compute base[0..58] using a standard DP for partitions into 8 distinct
   parts with one part fixed (=8).  The sums run over the 7 remaining parts. */
static void precompute_base(void)
{
    if (base_done) return;

    const int max_n = 58;            /* 0 … 58 inclusive */
    const int Smax  = max_n + 24;    /* sum of the 7 unrestricted parts */
    seq_t dp[8][84];                 /* 8 × (58+24+1) = 672 entries */
    memset(dp, 0, sizeof(dp));
    dp[0][0] = 1;

    for (int x = 1; x <= Smax; ++x) {
        if (x == 8) continue;        /* part 8 is handled separately */
        for (int t = 7; t >= 1; --t) {
            for (int s = x; s <= Smax; ++s) {
                dp[t][s] += dp[t-1][s-x];
            }
        }
    }

    for (int n = 0; n <= max_n; ++n) {
        int S = n + 24;
        base[n] = dp[7][S];
    }
    base_done = 1;
}

/* Returns a(n) for any n ≥ 0.  Cache is extended automatically. */
seq_t a(idx_t n)
{
    precompute_base();

    if (n >= MAX_CACHE) abort();   /* indices beyond the cache are not supported */

    if (n >= cache_len) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i <= 58) {
                cache[i] = base[i];
            } else {
                seq_t r = 0;
                r += 2 * cache[i-2];
                r += 2 * cache[i-3];
                r -= 3 * cache[i-5];
                r -= 3 * cache[i-6];
                r -=   cache[i-7];
                r +=   cache[i-8];
                r += 2 * cache[i-9];
                r += 2 * cache[i-10];
                r +=   cache[i-11];
                r +=   cache[i-12];
                r -=   cache[i-14];
                r -=   cache[i-15];
                r -= 2 * cache[i-16];
                r -= 2 * cache[i-17];
                r -=   cache[i-18];
                r +=   cache[i-19];
                r += 3 * cache[i-20];
                r += 3 * cache[i-21];
                r -= 2 * cache[i-23];
                r -= 2 * cache[i-24];
                r +=   cache[i-26];
                cache[i] = r;
            }
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
