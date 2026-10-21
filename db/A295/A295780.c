/*
 * A295780  Number of nX7 0..1 arrays with each 1 adjacent to 0 or 2 king-move
 *          neighboring 1s.
 * https://oeis.org/A295780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays where each 1 has 0 or 2
 *            king‑move neighboring 1s
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a row‑wise transfer‑matrix DP with a 2‑row state. */

#include <stdint.h>
#include <stdbool.h>

/* Precomputed masks: mask_lo[prev][curr] and mask_hi[prev][curr] contain bits of
 * all next rows that yield a valid triple (prev, curr, next). 0–63 in mask_lo,
 * 64–127 in mask_hi. */
static uint64_t mask_lo[128][128];
static uint64_t mask_hi[128][128];
static bool masks_precomputed = false;

/* Test whether all 1‑cells of curr have 0 or 2 neighboring 1s in the triple
 * (prev, curr, next). */
static bool valid_triple(unsigned int prev, unsigned int curr, unsigned int next)
{
    for (int j = 0; j < 7; ++j) {
        unsigned int bit = 1u << j;
        if ((curr & bit) == 0) continue;          /* only 1‑cells need check */
        unsigned int cnt = 0;
        /* upper neighbours */
        if (j > 0 && (prev & (1u << (j - 1)))) ++cnt;
        if (prev & bit)                      ++cnt;
        if (j < 6 && (prev & (1u << (j + 1)))) ++cnt;
        /* horizontal neighbours */
        if (j > 0 && (curr & (1u << (j - 1)))) ++cnt;
        if (j < 6 && (curr & (1u << (j + 1)))) ++cnt;
        /* lower neighbours */
        if (j > 0 && (next & (1u << (j - 1)))) ++cnt;
        if (next & bit)                     ++cnt;
        if (j < 6 && (next & (1u << (j + 1)))) ++cnt;
        if (cnt != 0 && cnt != 2) return false;
    }
    return true;
}

/* Compute all masks once. */
static void compute_masks(void)
{
    for (unsigned int prev = 0; prev < 128; ++prev) {
        for (unsigned int curr = 0; curr < 128; ++curr) {
            uint64_t lo = 0, hi = 0;
            for (unsigned int next = 0; next < 128; ++next) {
                if (valid_triple(prev, curr, next)) {
                    if (next < 64) lo |= 1ULL << next;
                    else           hi |= 1ULL << (next - 64);
                }
            }
            mask_lo[prev][curr] = lo;
            mask_hi[prev][curr] = hi;
        }
    }
    masks_precomputed = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but we guard anyway */

    if (!masks_precomputed) compute_masks();

    static seq_t dp[128][128];
    static seq_t dp_next[128][128];

    /* initialise dp for the first row (prev = 0). */
    for (int i = 0; i < 128; ++i)
        for (int j = 0; j < 128; ++j)
            dp[i][j] = 0;

    for (int curr = 0; curr < 128; ++curr)
        dp[0][curr] = 1;   /* one way to choose the first row */

    /* add subsequent rows */
    for (idx_t step = 1; step < n; ++step) {
        /* zero dp_next */
        for (int i = 0; i < 128; ++i)
            for (int j = 0; j < 128; ++j)
                dp_next[i][j] = 0;

        for (int prev = 0; prev < 128; ++prev) {
            for (int curr = 0; curr < 128; ++curr) {
                seq_t val = dp[prev][curr];
                if (val == 0) continue;

                /* iterate over all allowed next rows */
                uint64_t bits = mask_lo[prev][curr];
                while (bits) {
                    unsigned int next = __builtin_ctzll(bits);
                    dp_next[curr][next] += val;
                    bits &= bits - 1;
                }
                bits = mask_hi[prev][curr];
                while (bits) {
                    unsigned int next = __builtin_ctzll(bits) + 64;
                    dp_next[curr][next] += val;
                    bits &= bits - 1;
                }
            }
        }
        /* swap dp and dp_next */
        for (int i = 0; i < 128; ++i)
            for (int j = 0; j < 128; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* sum over states where the last row (curr) satisfies the bottom boundary
     * with next row all zeros (bit 0 of the mask). */
    seq_t ans = 0;
    for (int prev = 0; prev < 128; ++prev) {
        for (int curr = 0; curr < 128; ++curr) {
            if (mask_lo[prev][curr] & 1ULL) {
                ans += dp[prev][curr];
            }
        }
    }
    return ans;
}
