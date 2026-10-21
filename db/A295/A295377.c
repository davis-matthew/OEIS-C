/*
 * A295377  Number of nX6 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 2 or 4 1s.
 * https://oeis.org/A295377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary matrices where every 1 has an even
 *            number * (0, 2 or 4) of orthogonal 1‑neighbours. Implemented by
 *            dynamic programming * over rows using transition masks
 *            precomputed once for all n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint64_t next_mask_global[64][64];
static bool final_valid_global[64][64];

static void precompute_masks(void)
{
    static bool ready = false;
    if (ready) return;
    ready = true;

    for (uint64_t prev = 0; prev < 64; ++prev)
    {
        for (uint64_t cur = 0; cur < 64; ++cur)
        {
            uint64_t mask = 0;
            for (uint64_t next = 0; next < 64; ++next)
            {
                bool ok = true;
                for (int i = 0; i < 6 && ok; ++i)
                {
                    if ((cur >> i) & 1ULL)
                    {
                        int up   = (prev >> i) & 1;
                        int down = (next >> i) & 1;
                        int left = (i > 0)   ? ((cur >> (i - 1)) & 1) : 0;
                        int right= (i < 5)  ? ((cur >> (i + 1)) & 1) : 0;
                        if ((up ^ down ^ left ^ right) != 0) ok = false;
                    }
                }
                if (ok) mask |= 1ULL << next;
            }
            next_mask_global[prev][cur] = mask;
        }
    }

    for (uint64_t prev = 0; prev < 64; ++prev)
    {
        for (uint64_t cur = 0; cur < 64; ++cur)
        {
            bool ok = true;
            for (int i = 0; i < 6 && ok; ++i)
            {
                if ((cur >> i) & 1ULL)
                {
                    int up   = (prev >> i) & 1;
                    int down = 0;
                    int left = (i > 0)   ? ((cur >> (i - 1)) & 1) : 0;
                    int right= (i < 5)  ? ((cur >> (i + 1)) & 1) : 0;
                    if ((up ^ down ^ left ^ right) != 0) ok = false;
                }
            }
            final_valid_global[prev][cur] = ok;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset 1 – unused for n==0 */

    precompute_masks();

    static seq_t dp[64][64];
    static seq_t ndp[64][64];

    memset(dp, 0, sizeof(dp));
    memset(ndp, 0, sizeof(ndp));

    for (uint64_t r0 = 0; r0 < 64; ++r0)
        dp[0][r0] = 1;  /* first row, above row is all zeros */

    for (idx_t step = 1; step < n; ++step)
    {
        memset(ndp, 0, sizeof(ndp));

        for (uint64_t prev = 0; prev < 64; ++prev)
        {
            for (uint64_t cur  = 0; cur < 64; ++cur)
            {
                seq_t ways = dp[prev][cur];
                if (ways == 0) continue;

                uint64_t mask = next_mask_global[prev][cur];
                while (mask)
                {
                    uint64_t next = __builtin_ctzll(mask);
                    mask &= mask - 1;
                    ndp[cur][next] += ways;
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    seq_t result = 0;
    for (uint64_t prev = 0; prev < 64; ++prev)
    {
        for (uint64_t cur = 0; cur < 64; ++cur)
        {
            if (final_valid_global[prev][cur])
                result += dp[prev][cur];
        }
    }
    return result;
}
