/*
 * A244182  Number of length n+4 0..7 arrays with no five consecutive elements
 *          with pattern ababa or abbba (possibly a=b) and new values 0..7
 *          introduced in 0..7 order.
 * https://oeis.org/A244182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+4 arrays over 0..7 with no * 5
 *            consecutive elements forming pattern ababa or abbba (a and b may
 *            be equal). * Computed via dynamic programming over
 *            restricted‑growth strings. * Initial values (from A244182):
 *            a(1)=49, a(2)=189, a(3)=812,
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   unpack_last4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/unpack_last4.h"

#define MAX_SYMBOL 8
#define LAST4_BITS 12          /* 4 symbols * 3 bits each */
#define LAST4_SIZE (1u << LAST4_BITS) /* 4096 */
#define STATES (MAX_SYMBOL * LAST4_SIZE) /* 32768 */

static bool  init_done = false;
static seq_t *dp_cur   = NULL;   /* DP for current length */
static seq_t *dp_next  = NULL;   /* DP for next length */
static seq_t *totals   = NULL;   /* totals[length] = a(length-4) */
static size_t tot_cap  = 0;
static size_t tot_len  = 0;

/* Allocate or grow totals array */
static void add_total(seq_t val)
{
    if (tot_len >= tot_cap) {
        size_t new_cap = tot_cap ? tot_cap * 2 : 64;
        totals = (seq_t*)realloc(totals, new_cap * sizeof(seq_t));
        tot_cap = new_cap;
    }
    totals[tot_len++] = val;
}

/* One‑time initialization */
static void init_once(void)
{
    size_t i;
    /* Allocate DP arrays */
    dp_cur    = (seq_t*)calloc(STATES, sizeof(seq_t));
    dp_next   = (seq_t*)calloc(STATES, sizeof(seq_t));

    /* Enumerate all restricted‑growth strings of length 4 */
    for (int a = 0; a < MAX_SYMBOL; ++a)
        for (int b = 0; b < MAX_SYMBOL; ++b)
            for (int c = 0; c < MAX_SYMBOL; ++c)
                for (int d = 0; d < MAX_SYMBOL; ++d) {
                    if (a != 0) continue;               /* start at 0 */
                    int cur_max = 0;
                    bool ok = true;
                    int t[4] = {a,b,c,d};
                    for (int k = 0; k < 4; ++k) {
                        if (t[k] > cur_max + 1) { ok = false; break; }
                        if (t[k] > cur_max) cur_max = t[k];
                    }
                    if (!ok) continue;
                    uint32_t packed = ((a << 9) | (b << 6) | (c << 3) | d);
                    size_t s = (size_t)cur_max * LAST4_SIZE + packed;
                    dp_cur[s] = 1;
                }

    /* Sum totals for length 4 */
    seq_t sum = 0;
    for (i = 0; i < STATES; ++i) sum += dp_cur[i];

    tot_cap = 64;
    totals  = (seq_t*)calloc(tot_cap, sizeof(seq_t));
    tot_len = 5;          /* indices 0..4 are defined, 4 holds length 4 */
    totals[4] = sum;
    init_done = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 0 */

    if (!init_done) init_once();

    /* Desired array length */
    idx_t target_len = n + 4;

    /* Extend DP until we reach target length */
    while (tot_len <= (size_t)target_len) {
        /* Clear dp_next */
        memset(dp_next, 0, STATES * sizeof(seq_t));

        /* Transition from all current states */
        for (size_t s = 0; s < STATES; ++s) {
            seq_t cnt = dp_cur[s];
            if (cnt == 0) continue;

            int cur_max = (int)(s >> LAST4_BITS);      /* high 3 bits */
            uint32_t last4 = (uint32_t)(s & (LAST4_SIZE - 1));

            int a0, b0, c0, d0;
            unpack_last4(last4, &a0, &b0, &c0, &d0);

            for (int v = 0; v < MAX_SYMBOL; ++v) {
                /* Restricted‑growth condition */
                if (v > cur_max + 1) continue;
                int new_max = (v > cur_max) ? cur_max + 1 : cur_max;

                /* Forbidden patterns */
                if ((a0 == c0 && b0 == d0 && v == a0)          /* ababa */
                    || (b0 == c0 && c0 == d0 && v == a0))      /* abbba */
                    continue;

                /* Pack new last four symbols */
                uint32_t new_last4 = (((last4 & 0x1FFu) << 3) | (uint32_t)v);
                size_t new_state = (size_t)new_max * LAST4_SIZE + new_last4;

                dp_next[new_state] += cnt;
            }
        }

        /* Sum totals for next length */
        seq_t sum = 0;
        for (size_t i = 0; i < STATES; ++i) sum += dp_next[i];
        add_total(sum);

        /* Swap DP arrays */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    return totals[target_len];
}
