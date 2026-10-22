/*
 * A345476  Numbers that are the sum of six squares in nine or more ways.
 * https://oeis.org/A345476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 161, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A345476: Numbers that are the sum of six positive squares
   in nine or more unordered ways.  The function counts, for each integer,
   the number of unordered 6‑tuples (a,b,c,d,e,f) of positive integers
   with a²+b²+c²+d²+e²+f² = n;  if this count is at least 9, the integer
   is included in the sequence.  The nth term (offset 1) is returned by
   the function a.  The routine builds the sequence once at first call
   and reuses the cached data thereafter.  */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* integer square root for idx_t (uint64_t) */
static idx_t _isqrt_idx(idx_t x)
{
    idx_t r = (idx_t)sqrt((double)x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* cached data for the sequence */
static seq_t *seq_values = NULL;    /* array of terms */
static idx_t seq_len = 0;           /* number of stored terms */

/* build the sequence once */
static void build_sequence(void)
{
    /* upper bound for values to examine – sufficient for thousands of terms */
    const idx_t MAX_N = 200000;
    const int K = 6;                /* number of squares in each sum */

    /* dp[sum * (K+1) + k] gives number of ways to form 'sum' using exactly
       k positive square summands, capped at 9 to save storage.  */
    const size_t DP_SIZE = (size_t)(MAX_N + 1) * (K + 1);
    uint8_t *dp = (uint8_t *)calloc(DP_SIZE, sizeof(uint8_t));
    if (!dp) return;                 /* out of memory – graceful fallback */

    dp[0 * (K + 1) + 0] = 1;          /* empty representation of 0 */

    idx_t max_s = _isqrt_idx(MAX_N);
    for (idx_t s = 1; s <= max_s; ++s) {
        idx_t sq = s * s;
        for (idx_t sum = sq; sum <= MAX_N; ++sum) {
            size_t base = (size_t)sum * (K + 1);
            size_t src  = (size_t)(sum - sq) * (K + 1);
            for (int k = 1; k <= K; ++k) {
                uint8_t add = dp[src + k - 1];
                if (!add) continue;
                uint8_t cur = dp[base + k];
                uint16_t newval = cur + add;
                if (newval > 9) newval = 9;
                dp[base + k] = (uint8_t)newval;
            }
        }
    }

    /* collect terms with at least nine representations */
    seq_t *buf = NULL;
    size_t cap = 0;
    for (idx_t v = 1; v <= MAX_N; ++v) {
        uint8_t ways = dp[(size_t)v * (K + 1) + K];
        if (ways >= 9) {
            if (seq_len >= cap) {
                cap = cap ? cap * 2 : 64;
                seq_values = (seq_t *)realloc(seq_values, cap * sizeof(seq_t));
                if (!seq_values) { free(dp); return; }
            }
            seq_values[seq_len++] = (seq_t)v;
        }
    }

    free(dp);
}

/* Public function, external linkage */
seq_t a(idx_t n)
{
    static bool built = false;
    if (!built) {
        build_sequence();
        built = true;
    }
    if (n == 0) return 0;
    if (n - 1 < seq_len) return seq_values[n - 1];
    return 0;   /* if n is beyond precomputed range, return 0 */
}
