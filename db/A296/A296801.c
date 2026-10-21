/*
 * A296801  Number of nX5 0..1 arrays with each 1 adjacent to 1, 3 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   counts 0..1 arrays of size n×5 each 1 has 1,3 or 4 king‑move
 *            neighbours
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool valid_row(int prev, int cur, int next) {
    /* check for every column that has a 1 in cur */
    for (int col = 0; col < 5; col++) {
        if (!((cur >> col) & 1)) continue;
        int count = 0;
        for (int dr = -1; dr <= 1; dr++) {
            int rowbits = (dr == -1) ? prev : (dr == 0 ? cur : next);
            for (int dc = -1; dc <= 1; dc++) {
                if (dr == 0 && dc == 0) continue;
                int c = col + dc;
                if (c < 0 || c >= 5) continue;
                if ((rowbits >> c) & 1) count++;
            }
        }
        if (count != 1 && count != 3 && count != 4) return false;
    }
    return true;
}

static void init_masks(uint32_t nextMask[32][32]) {
    for (int prev = 0; prev < 32; prev++) {
        for (int cur = 0; cur < 32; cur++) {
            uint32_t mask = 0;
            for (int next = 0; next < 32; next++) {
                if (valid_row(prev, cur, next))
                    mask |= 1u << next;
            }
            nextMask[prev][cur] = mask;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1, but never called with 0 */

    /* precompute transition masks once */
    static bool ready = false;
    static uint32_t nextMask[32][32];
    if (!ready) {
        init_masks(nextMask);
        ready = true;
    }

    const size_t STATES = 32U * 32U;           /* (prev<<5)|cur  */
    seq_t *dp = (seq_t *)malloc(STATES * sizeof(seq_t));
    seq_t *newdp = (seq_t *)malloc(STATES * sizeof(seq_t));
    if (!dp || !newdp) return 0;              /* allocation failure (unlikely) */

    memset(dp, 0, STATES * sizeof(seq_t));
    seq_t one = 1;
    for (int r = 0; r < 32; r++)
        dp[(0 << 5) | r] = one;              /* start with prev = 0 */

    for (idx_t step = 2; step <= n; ++step) {
        memset(newdp, 0, STATES * sizeof(seq_t));
        for (size_t idx = 0; idx < STATES; ++idx) {
            seq_t val = dp[idx];
            if (val == 0) continue;
            int prev = idx >> 5;
            int cur  = idx & 31;
            uint32_t mask = nextMask[prev][cur];
            while (mask) {
                int next = __builtin_ctz(mask);
                mask &= mask - 1;
                size_t nidx = (cur << 5) | next;
                newdp[nidx] += val;
            }
        }
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t ans = 0;
    for (size_t idx = 0; idx < STATES; ++idx) {
        seq_t val = dp[idx];
        if (val == 0) continue;
        int prev = idx >> 5;
        int cur  = idx & 31;
        if (nextMask[prev][cur] & 1u)  /* next = 0 is allowed → bottom row OK */
            ans += val;
    }

    free(dp);
    free(newdp);
    return ans;
}
