/*
 * A297811  Number of nX5 0..1 arrays with every element equal to 2 or 3
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 62, u128 n <= 124,
 *            big any n
 * Helpers:   bool_same, check_bottom, check_mid, check_top
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A297811 – Number of n×5 binary arrays with each element having
 * exactly 2 or 3 same-valued king-move neighbors (no row above for the
 * first row, no row below for the last row).  Upper‑left element is zero.
 * 
 * The sequence satisfies a linear recurrence for n > 21:
 *   a(n) =  2*a(n‑1) + 1*a(n‑2) - 2*a(n‑3) + 4*a(n‑4) - 4*a(n‑5)
 *          -8*a(n‑6) - 1*a(n‑7) + 5*a(n‑8) + 1*a(n‑9) + 4*a(n‑10)
 *          +1*a(n‑11) - 3*a(n‑12)
 * Initial values 1 ≤ n ≤ 21 are computed by dynamic programming on rows.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/bool_same.h"
#include "../utils/check_bottom.h"
#include "../utils/check_mid.h"
#include "../utils/check_top.h"

/* ----------  helpers for bit handling  ---------- */

/* ----------  check functions  ---------- */

/* ----------  pre‑computed tables  ---------- */

static uint8_t  validTop[32][32];
static uint8_t  validBottom[32][32];
static uint32_t validMidMask[32][32];   /* bitmask of next rows */

static int tables_initialized = 0;

static void init_tables(void) {
    if (tables_initialized) return;

    for (int cur = 0; cur < 32; cur++) {
        for (int next = 0; next < 32; next++) {
            validTop[cur][next] = check_top(cur, next);
        }
    }

    for (int prev = 0; prev < 32; prev++) {
        for (int curr = 0; curr < 32; curr++) {
            validBottom[prev][curr] = check_bottom(prev, curr);
            uint32_t mask = 0;
            for (int next = 0; next < 32; next++) {
                if (check_mid(prev, curr, next))
                    mask |= 1u << next;
            }
            validMidMask[prev][curr] = mask;
        }
    }
    tables_initialized = 1;
}

/* ----------  DP for a given n (n ≥ 2)  ---------- */

static seq_t compute_term_dp(idx_t n) {
    /* n >= 2 */
    init_tables();

    static seq_t dp[32][32];
    static seq_t newdp[32][32];

    /* clear */
    for (int i = 0; i < 32; i++)
        for (int j = 0; j < 32; j++)
            dp[i][j] = 0;

    /* first two rows */
    for (int first = 0; first < 32; first++) {
        if (first & 1) continue;          /* upper‑left zero */
        for (int second = 0; second < 32; second++) {
            if (validTop[first][second])
                dp[first][second] = 1;
        }
    }

    /* add intermediate rows (if any) */
    for (int step = 2; step < n; step++) {   /* rows 2 .. n-1 */
        /* clear newdp */
        for (int i = 0; i < 32; i++)
            for (int j = 0; j < 32; j++)
                newdp[i][j] = 0;

        for (int prev = 0; prev < 32; prev++) {
            for (int curr = 0; curr < 32; curr++) {
                if (dp[prev][curr] == 0) continue;
                uint32_t mask = validMidMask[prev][curr];
                while (mask) {
                    int next = __builtin_ctz(mask);
                    newdp[curr][next] += dp[prev][curr];
                    mask &= mask - 1;
                }
            }
        }
        /* copy newdp to dp */
        for (int i = 0; i < 32; i++)
            for (int j = 0; j < 32; j++)
                dp[i][j] = newdp[i][j];
    }

    /* bottom row check and total */
    seq_t total = 0;
    for (int prev = 0; prev < 32; prev++) {
        for (int curr = 0; curr < 32; curr++) {
            if (validBottom[prev][curr])
                total += dp[prev][curr];
        }
    }
    return total;
}

/* ----------  main sequence function  ---------- */

seq_t a(idx_t n) {
    if (n <= 1) return 0;   /* a(1) = 0 */

    /* cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* highest index cached (1‑based) */

    /* build initial cache up to 21 by DP */
    if (cache_len < 21) {
        size_t new_cap = 22;  /* indices 0..21, we ignore index 0 */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure */
        for (size_t i = 1; i <= 21; i++)
            cache[i] = compute_term_dp((idx_t)i);
        cache_len = 21;
    }

    if ((idx_t)n <= cache_len)
        return cache[(size_t)n];

    /* extend cache using recurrence for n > 21 */
    while ((idx_t)cache_len < n) {
        ++cache_len;
        if (cache_len <= 21) continue;   /* already computed */

        /* recurrence coefficients:
         * 2, 1, -2, 4, -4, -8, -1, 5, 1, 4, 1, -3  (for a(n-1)...a(n-12))
         */
        seq_t pos = 0, neg = 0;
        pos += 2 * cache[cache_len - 1];
        pos += 1 * cache[cache_len - 2];
        pos += 4 * cache[cache_len - 4];
        pos += 5 * cache[cache_len - 8];
        pos += 1 * cache[cache_len - 9];
        pos += 4 * cache[cache_len -10];
        pos += 1 * cache[cache_len -11];

        neg += 2 * cache[cache_len - 3];
        neg += 4 * cache[cache_len - 5];
        neg += 8 * cache[cache_len - 6];
        neg += 1 * cache[cache_len - 7];
        neg += 3 * cache[cache_len -12];

        cache[cache_len] = pos - neg;
    }
    return cache[(size_t)n];
}
