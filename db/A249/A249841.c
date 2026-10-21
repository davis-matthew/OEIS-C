/*
 * A249841  Number of length n+4 0..5 arrays with no five consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A249841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+4 arrays over {0,1,2,3,4,5} with * no
 *            five consecutive terms having the maximum of any two terms equal
 *            * to the minimum of the remaining three terms
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 24, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t *a_cache = NULL;          /* cache of a(m) for length m = n+4 */
static idx_t cache_len = 0;            /* allocated length of a_cache       */

/* State: 4 digits a,b,c,d (each 0..5).  Encode as
 * (((a*6)+b)*6+c)*6+d.  Total states = 6^4 = 1296.            */
#define STATE_COUNT 1296

/* Precomputed transitions and validity for each state and new digit. */
static bool transition_valid[STATE_COUNT][6];
static uint16_t next_state[STATE_COUNT][6];
static uint8_t dig0[STATE_COUNT], dig1[STATE_COUNT],
               dig2[STATE_COUNT], dig3[STATE_COUNT];

/* DP buffers for current and previous length. */
static seq_t *dp_prev = NULL;
static seq_t *dp_curr = NULL;
static idx_t dp_len = 0;   /* last computed length (>=4) */

/* Initialize static tables once. */
static void init_static(void)
{
    static bool already = false;
    if (already) return;
    already = true;

    /* Precompute digit decompositions for all states. */
    for (uint32_t a = 0; a < 6; ++a)
        for (uint32_t b = 0; b < 6; ++b)
            for (uint32_t c = 0; c < 6; ++c)
                for (uint32_t d = 0; d < 6; ++d) {
                    uint32_t s = ((((a * 6U) + b) * 6U + c) * 6U + d);
                    dig0[s] = (uint8_t)a;
                    dig1[s] = (uint8_t)b;
                    dig2[s] = (uint8_t)c;
                    dig3[s] = (uint8_t)d;
                }

    /* Precompute transitions and validity for each state and new digit. */
    for (uint32_t s = 0; s < STATE_COUNT; ++s) {
        uint8_t a = dig0[s];
        uint8_t b = dig1[s];
        uint8_t c = dig2[s];
        uint8_t d = dig3[s];
        for (uint8_t e = 0; e < 6; ++e) {
            uint8_t w[5] = {a, b, c, d, e};
            bool bad = false;
            /* Check every pair of indices 0..4 */
            for (uint8_t i = 0; i < 5 && !bad; ++i)
                for (uint8_t j = i + 1; j < 5 && !bad; ++j) {
                    /* max of the two chosen terms */
                    uint8_t mx = w[i] > w[j] ? w[i] : w[j];
                    /* min of the remaining three */
                    uint8_t mn = 255;
                    for (uint8_t k = 0; k < 5; ++k)
                        if (k != i && k != j)
                            if (w[k] < mn) mn = w[k];
                    if (mx == mn) bad = true;
                }
            transition_valid[s][e] = !bad;
            /* Next state after adding e: last 4 digits are b,c,d,e */
            uint32_t ns = (((b * 6U + c) * 6U + d) * 6U + e);
            next_state[s][e] = (uint16_t)ns;
        }
    }

    /* Allocate DP buffers. */
    dp_prev = (seq_t*)malloc(STATE_COUNT * sizeof(seq_t));
    dp_curr = (seq_t*)malloc(STATE_COUNT * sizeof(seq_t));
    if (!dp_prev || !dp_curr) exit(1);

    /* Start from length 4: all 1296 4‑tuples are allowed. */
    for (size_t i = 0; i < STATE_COUNT; ++i) dp_prev[i] = (seq_t)1;
    dp_len = 4;   /* length of sequences in dp_prev */

    /* Ensure cache at least of size 5 (index 4). */
    idx_t needed = 5;
    a_cache = (seq_t*)malloc(needed * sizeof(seq_t));
    if (!a_cache) exit(1);
    memset(a_cache, 0, needed * sizeof(seq_t));
    a_cache[4] = (seq_t)STATE_COUNT;   /* count for length 4 */
    cache_len = needed;
}

/* Compute a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    /*  Length of the array to count is m = n+4.  */
    idx_t m = n + 4;

    init_static();

    /* Extend the cache and DP if needed. */
    if (m >= cache_len) {
        idx_t new_len = m + 1;
        a_cache = (seq_t*)realloc(a_cache, new_len * sizeof(seq_t));
        if (!a_cache) exit(1);
        memset(a_cache + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        cache_len = new_len;
    }

    /* DP from current dp_len up to m. */
    while (dp_len < m) {
        /* Zero dp_curr for new length. */
        memset(dp_curr, 0, STATE_COUNT * sizeof(seq_t));

        /* Transition from all states of previous length. */
        for (size_t s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_prev[s];
            if (val == 0) continue;
            for (uint8_t e = 0; e < 6; ++e) {
                if (!transition_valid[s][e]) continue;
                uint16_t ns = next_state[s][e];
                dp_curr[ns] += val;
            }
        }

        /* Sum counts of all states to get total for new length. */
        seq_t total = 0;
        for (size_t i = 0; i < STATE_COUNT; ++i) total += dp_curr[i];
        ++dp_len;
        a_cache[dp_len] = total;

        /* Swap dp_prev and dp_curr for next iteration. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    return a_cache[m];
}
