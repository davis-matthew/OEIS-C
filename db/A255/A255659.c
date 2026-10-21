/*
 * A255659  Number of length n+7 0..3 arrays with at most two downsteps in
 *          every 7 consecutive neighbor pairs.
 * https://oeis.org/A255659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 11, u64 n <= 30, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: DP over a sliding window of 7 digits.  For each length
   L ≥ 8 we store the number of admissible arrays of length L.  The state
   space consists of all 4^7 = 16384 possible 7‑digit windows.  For every
   state we pre‑compute which next digit 0…3 can be appended while keeping
   at most two downsteps among the 7 consecutive neighbour pairs.  The
   result sequence a(n) (n ≥ 1) is the sum of all admissible states for
   array length L = n+7.  The function memoises all seen values so that
   calls may arrive in arbitrary order. */ 

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define STATE_COUNT 16384   /* 4^7 */
#define INVALID_TRANS (UINT16_MAX)

/* pre‑computed transition table, the entry is either a new state
   index (0 … STATE_COUNT-1) or INVALID_TRANS for an illegal
   transition. */
static uint16_t transition_table[STATE_COUNT][4];
static bool transition_ready = false;

/* Apply the pre‑computation once. */
static void init_transition(void)
{
    if (transition_ready) return;
    for (int s = 0; s < STATE_COUNT; ++s) {
        /* extract the 7 digits: d0 is most significant, d6 least */
        int digits[7];
        int tmp = s;
        for (int i = 6; i >= 0; --i) {
            digits[i] = tmp & 3;      /* 2 bits per digit */
            tmp >>= 2;
        }
        /* count downsteps among the 6 internal pairs */
        int downs = 0;
        for (int i = 0; i < 6; ++i)
            if (digits[i] > digits[i+1]) ++downs;
        /* try the 4 possible extension digits */
        for (int v = 0; v < 4; ++v) {
            int total_downs = downs + (digits[6] > v ? 1 : 0);
            if (total_downs <= 2) {
                /* drop the oldest digit (digits[0]) and append v */
                uint16_t new_state = ((s & 0x0FFF) << 2) | (uint16_t)v;
                transition_table[s][v] = new_state;
            } else {
                transition_table[s][v] = INVALID_TRANS;
            }
        }
    }
    transition_ready = true;
}

/* DP arrays for the two alternating stages. */
static seq_t dp[2][STATE_COUNT];
static unsigned char dp_parity = 0;   /* 0 or 1 */

/* Cache of computed sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Current processed array length (number of symbols). */
static idx_t current_len = 7;  /* start at length 7, before first transition */

/* Ensure cache has at least n terms (n ≥ 1). */
static void ensure_upto(idx_t n)
{
    if (n == 0) return;                /* no terms needed */

    if (!transition_ready) init_transition();

    if (!cache) {   /* first time: initialise DP and cache */
        cache_cap = 64;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        cache_len = 0;

        for (idx_t i = 0; i < STATE_COUNT; ++i) {
            dp[0][i] = (seq_t)1;        /* each 7‑digit pattern is a distinct length‑7 array */
            dp[1][i] = (seq_t)0;
        }
        dp_parity = 0;
        current_len = 7;
    }

    idx_t target_len = n + 7;          /* array length needed for term a(n) */
    while (current_len < target_len) {
        /* clear next stage */
        memset(dp[dp_parity ^ 1], 0, sizeof(dp[dp_parity ^ 1]));

        /* transition */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp[dp_parity][s];
            if (!val) continue;
            for (int v = 0; v < 4; ++v) {
                uint16_t t = transition_table[s][v];
                if (t != INVALID_TRANS) {
                    dp[dp_parity ^ 1][t] += val;
                }
            }
        }

        dp_parity ^= 1;
        ++current_len;

        /* sum all counts to obtain a(current_len - 7) */
        seq_t sum = 0;
        for (int i = 0; i < STATE_COUNT; ++i) {
            sum += dp[dp_parity][i];
        }

        /* extend cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = new_ptr;
            cache_cap = new_cap;
        }
        cache[cache_len++] = sum;
    }
}

/* ----- public entry point ----- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, return 0 safely */
    ensure_upto(n);
    return cache[(size_t)(n - 1)];  /* cache is 0‑based: a(1) at index 0 */
}
