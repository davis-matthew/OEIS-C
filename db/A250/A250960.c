/*
 * A250960  Number of (n+1)X(3+1) 0..3 arrays with no 2X2 subblock having its
 *          minimum diagonal element less than its minimum antidiagonal
 *          element.
 * https://oeis.org/A250960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A250960
    Number of (n+1)×(3+1) 0..3 arrays with no 2×2 subblock having
    its minimum diagonal element less than its minimum antidiagonal element.
    The implementation uses dynamic programming over the 256 possible
    4‑digit rows.  For each pair of rows we precompute the admissible
    transitions, building a directed graph.  The number of valid arrays
    equals the number of walks of length n in this graph.
*/

#include <stdbool.h>

/* Transition tables: for each source row (0..255) the list of admissible
   destination rows and the count of such rows.  next_cnt must hold values
   up to 256, therefore it is uint16_t. */
static uint16_t next_buf[256][256];
static uint16_t next_cnt[256];
static bool trans_initialized = false;

/* Pre‑compute transitions for all 256 rows. */
static void init_transition(void)
{
    for (int s = 0; s < 256; ++s) {
        uint8_t a[4];
        for (int c = 0; c < 4; ++c)
            a[c] = (s >> (2 * c)) & 3U;

        uint16_t cnt = 0;
        for (int t = 0; t < 256; ++t) {
            uint8_t b[4];
            for (int c = 0; c < 4; ++c)
                b[c] = (t >> (2 * c)) & 3U;

            bool ok = true;
            for (int c = 0; c < 3; ++c) {
                uint8_t diagMin = a[c] < b[c + 1] ? a[c] : b[c + 1];
                uint8_t antiMin = b[c] < a[c + 1] ? b[c] : a[c + 1];
                if (diagMin < antiMin) { ok = false; break; }
            }
            if (ok) next_buf[s][cnt++] = (uint16_t)t;
        }
        next_cnt[s] = cnt;
    }
    trans_initialized = true;
}

/* DP tables for current and next step: 256 rows each. */
static seq_t dp_cur[256];
static seq_t dp_next[256];

/* Sequence function: a(n) for n >= 1 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset 1 – values for n ≥ 1 only */

    if (!trans_initialized) init_transition();

    /* Initialise: one way to start in each row. */
    for (int i = 0; i < 256; ++i) dp_cur[i] = (seq_t)1;

    for (idx_t step = 1; step <= n; ++step) {
        /* Zero the next table. */
        for (int i = 0; i < 256; ++i) dp_next[i] = (seq_t)0;

        /* Propagate transitions. */
        for (int s = 0; s < 256; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;

            uint16_t cnt = next_cnt[s];
            for (uint16_t k = 0; k < cnt; ++k) {
                int t = next_buf[s][k];
                dp_next[t] += val;
            }
        }

        /* Copy dp_next into dp_cur for next iteration. */
        for (int i = 0; i < 256; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum counts of all ending rows. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 256; ++i) result += dp_cur[i];
    return result;
}
