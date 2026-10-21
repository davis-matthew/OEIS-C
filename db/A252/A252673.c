/*
 * A252673  Number of (n+2) X (2+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0, 2, 5, 6 or 7 and every 3 X 3 column
 *          and antidiagonal sum not equal to 0, 2, 5, 6 or 7.
 * https://oeis.org/A252673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n−3)
 *            −178*a(n−6)+713*a(n−9)−1673*a(n−12)+2744*a(n−15)
 *            −3188*a(n−18)+2096*a(n−21)−668*a(n−24)+103*a(n−27)+38*a(n−30)
 *            −8*a(n−33) (n>37) Initial values a(1..37) are computed by a
 *            depth‑first search respecting the 0..3 4×4 array constraints.
 *            (The function implements the recurrence for n>37.)
 * Range:     u8 none, u16 n <= 9, u32 n <= 23, u64 n <= 53, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* idx_t and seq_t are supplied by the build environment. */

#define STATE_COUNT   65536
#define MAX_N          10000

/* Transition tables for the DP */
static bool initialized = false;
static bool allowed[10];
static unsigned char *next_rows[STATE_COUNT];
static uint16_t next_cnt[STATE_COUNT];
static unsigned char *next_rows_data = NULL;

/* Precompute all allowed 3‑row transitions */
static void init_tables(void)
{
    if (initialized) return;

    /* Allowed sums: 0,2,5,6,7 */
    for (int i = 0; i < 10; ++i) allowed[i] = false;
    allowed[0] = allowed[2] = allowed[5] = allowed[6] = allowed[7] = true;

    /* Allocate storage: STATE_COUNT * 256 bytes (max one possibility per state) */
    next_rows_data = (unsigned char *)malloc((size_t)STATE_COUNT * 256);
    if (!next_rows_data) exit(1);

    for (int prev2 = 0; prev2 < 256; ++prev2) {
        for (int prev1 = 0; prev1 < 256; ++prev1) {
            int state = (prev2 << 8) | prev1;
            unsigned char *list = next_rows_data + (size_t)state * 256;
            uint16_t count = 0;

            /* Try every possible third row */
            for (int r3 = 0; r3 < 256; ++r3) {
                bool ok = true;

                /* Check the 2 × 2 blocks (positions s=0,1) only, because array is
                   (n+2)×4.  For each s, rows [prev2,prev1,r3] form a 3×3 subblock. */
                for (int s = 0; s < 2 && ok; ++s) {
                    /* Row sums of the 3x3 subblocks */
                    int sum0 = ((prev2 >> (2 * s)) & 3) + ((prev2 >> (2 * (s + 1))) & 3) + ((prev2 >> (2 * (s + 2))) & 3);
                    int sum1 = ((prev1 >> (2 * s)) & 3) + ((prev1 >> (2 * (s + 1))) & 3) + ((prev1 >> (2 * (s + 2))) & 3);
                    int sum2 = ((r3    >> (2 * s)) & 3) + ((r3    >> (2 * (s + 1))) & 3) + ((r3    >> (2 * (s + 2))) & 3);
                    if (!allowed[sum0] || !allowed[sum1] || !allowed[sum2]) { ok = false; break; }

                    /* Diagonal sum of each 3x3 subblock */
                    int diag = ((prev2 >> (2 * s)) & 3) + ((prev1 >> (2 * (s + 1))) & 3) + ((r3 >> (2 * (s + 2))) & 3);
                    if (!allowed[diag]) { ok = false; break; }

                    /* Column sums of the 3x3 subblocks must not be allowed */
                    for (int col = 0; col < 3; ++col) {
                        int colsum = ((prev2 >> (2 * (s + col))) & 3) +
                                     ((prev1 >> (2 * (s + col))) & 3) +
                                     ((r3   >> (2 * (s + col))) & 3);
                        if (allowed[colsum]) { ok = false; break; }
                    }
                    if (!ok) break;

                    /* Anti‑diagonal sum must not be allowed */
                    int antidiag = ((prev2 >> (2 * (s + 2))) & 3) +
                                   ((prev1 >> (2 * (s + 1))) & 3) +
                                   ((r3   >> (2 * s)) & 3);
                    if (allowed[antidiag]) { ok = false; break; }
                }

                if (ok) {
                    list[count++] = (unsigned char)r3;
                }
            }
            next_rows[state] = list;
            next_cnt[state] = count;
        }
    }

    initialized = true;
}

/* DP for n ≤ 37 (height H = n+2) */
static seq_t compute_small(idx_t n)
{
    init_tables();

    uint32_t H = (uint32_t)n + 2;   /* total number of rows */
    static seq_t dp[2][STATE_COUNT];
    uint32_t cur = 0, nxt = 1;

    /* initial: any pair of first two rows is allowed */
    for (int i = 0; i < STATE_COUNT; ++i) dp[cur][i] = 1;

    for (uint32_t step = 3; step <= H; ++step) {
        memset(dp[nxt], 0, sizeof(dp[nxt]));
        for (int state = 0; state < STATE_COUNT; ++state) {
            seq_t val = dp[cur][state];
            if (!val) continue;
            uint16_t cnt = next_cnt[state];
            unsigned char *list = next_rows[state];
            for (int k = 0; k < cnt; ++k) {
                unsigned char r3 = list[k];
                int new_state = ((state & 0xFF) << 8) | r3;
                dp[nxt][new_state] += val;
            }
        }
        uint32_t tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum over all final states */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i) total += dp[cur][i];
    return total;
}

/* Cache for computed values */
static seq_t *seq_cache = NULL;
static idx_t cache_size = 0;
static idx_t cached_max = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* below offset, irrelevant */

    /* allocate or extend cache */
    if (seq_cache == NULL) {
        cache_size = MAX_N + 1;
        seq_cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
        if (!seq_cache) exit(1);
    }
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        memset(tmp + cache_size, 0, (size_t)(new_size - cache_size) * sizeof(seq_t));
        seq_cache = tmp;
        cache_size = new_size;
    }

    /* compute terms up to n */
    if (cached_max < 1) {
        /* first 37 terms by DP */
        for (idx_t i = 1; i <= 37 && i <= n; ++i) {
            seq_cache[i] = compute_small(i);
        }
        cached_max = 37;
    }
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i <= 37) {
            seq_cache[i] = compute_small(i);
        } else {
            seq_t pos = 0;
            seq_t neg = 0;
            pos += ((seq_t)22) * seq_cache[i-3];
            pos += ((seq_t)713) * seq_cache[i-9];
            pos += ((seq_t)2744) * seq_cache[i-15];
            pos += ((seq_t)2096) * seq_cache[i-21];
            pos += ((seq_t)103) * seq_cache[i-27];
            pos += ((seq_t)38)  * seq_cache[i-30];

            neg += ((seq_t)178) * seq_cache[i-6];
            neg += ((seq_t)1673) * seq_cache[i-12];
            neg += ((seq_t)3188) * seq_cache[i-18];
            neg += ((seq_t)668)  * seq_cache[i-24];
            neg += ((seq_t)8)    * seq_cache[i-33];       /* missing term added */

            seq_cache[i] = pos - neg;
        }
    }
    cached_max = n;
    return seq_cache[n];
}
