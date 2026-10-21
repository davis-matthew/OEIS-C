/*
 * A249230  Number of length n+5 0..5 arrays with no six consecutive terms
 *          having two times the sum of any two elements equal to the sum of
 *          the remaining four.
 * https://oeis.org/A249230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+5 arrays over {0,1,2,3,4,5} with no 6
 *            consecutive terms having two times the sum of any two elements
 *            equal to the sum of the remaining four
 * Range:     u8 none, u16 n <= 1, u32 n <= 9, u64 n <= 24, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define STATE_COUNT 7776  /* 6^5 */

/* Transition tables (built once) */
static unsigned char next_cnt[STATE_COUNT];
static int next_to[STATE_COUNT][6];
static bool initialized = false;

/* DP buffers (built once) */
static seq_t *dp_cur = NULL;
static seq_t *dp_nxt = NULL;

/* Build transition tables on first call */
static void init_tables(void)
{
    if (initialized) return;
    initialized = true;

    /* Allocate dp buffers */
    dp_cur = (seq_t*)malloc(sizeof(seq_t) * STATE_COUNT);
    dp_nxt = (seq_t*)malloc(sizeof(seq_t) * STATE_COUNT);

    int d[6];
    for (int d0 = 0; d0 < 6; ++d0)
        for (int d1 = 0; d1 < 6; ++d1)
            for (int d2 = 0; d2 < 6; ++d2)
                for (int d3 = 0; d3 < 6; ++d3)
                    for (int d4 = 0; d4 < 6; ++d4)
                    {
                        int idx = ((((d0)*6 + d1)*6 + d2)*6 + d3)*6 + d4;
                        int sum5 = d0 + d1 + d2 + d3 + d4;
                        d[0] = d0; d[1] = d1; d[2] = d2; d[3] = d3; d[4] = d4;
                        unsigned char cnt = 0;
                        for (int v = 0; v < 6; ++v)
                        {
                            int S = sum5 + v;
                            d[5] = v;
                            bool bad = false;
                            for (int p = 0; p < 6 && !bad; ++p)
                                for (int q = p + 1; q < 6; ++q)
                                {
                                    int sum_pair = d[p] + d[q];
                                    if (3 * sum_pair == S) { bad = true; break; }
                                }
                            if (!bad)
                            {
                                int nxt = ((((d1)*6 + d2)*6 + d3)*6 + d4)*6 + v;
                                next_to[idx][cnt] = nxt;
                                ++cnt;
                            }
                        }
                        next_cnt[idx] = cnt;
                    }
}

/* Main counting function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* off‑offset behaviour irrelevant */

    init_tables();

    /* start with all 5‑length prefixes */
    for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 1;

    /* perform n extensions (from length 5 to length 5+n) */
    for (idx_t step = 0; step < n; ++step)
    {
        memset(dp_nxt, 0, sizeof(seq_t) * STATE_COUNT);
        for (int i = 0; i < STATE_COUNT; ++i)
        {
            seq_t val = dp_cur[i];
            if (val == 0) continue;
            unsigned char cnt = next_cnt[i];
            int *tgt = next_to[i];
            for (unsigned char t = 0; t < cnt; ++t)
                dp_nxt[tgt[t]] += val;
        }
        /* swap buffers */
        seq_t *tmp = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;
    }

    /* sum all ending states */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i) total += dp_cur[i];
    return total;
}
