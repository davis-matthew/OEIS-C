/*
 * A296112  Number of n X 5 0..1 arrays with each 1 adjacent to 3 or 4
 *          king-move neighboring 1's.
 * https://oeis.org/A296112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming over rows for a 5‑column board.
   The DP enumerates all possible rows and transitions that
   satisfy the king‑move adjacency condition.
   Complexity: O(n · 1024) with a small constant factor. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* transition validity test for a single cell */
static bool valid_transition(uint32_t above, uint32_t current, uint32_t below)
{
    /* bits 0..4 correspond to columns 0..4 */
    for (uint32_t pos = 0; pos < 5u; ++pos)
    {
        if ((current >> pos) & 1U)          /* cell is 1 */
        {
            unsigned cnt = 0;
            /* horizontal neighbours in the same row */
            if (pos > 0u && (current >> (pos - 1u)) & 1U) ++cnt;
            if (pos < 4u && (current >> (pos + 1u)) & 1U) ++cnt;
            /* upper neighbours */
            if ((above >> pos) & 1U) ++cnt;
            if (pos > 0u && (above >> (pos - 1u)) & 1U) ++cnt;
            if (pos < 4u && (above >> (pos + 1u)) & 1U) ++cnt;
            /* lower neighbours */
            if ((below >> pos) & 1U) ++cnt;
            if (pos > 0u && (below >> (pos - 1u)) & 1U) ++cnt;
            if (pos < 4u && (below >> (pos + 1u)) & 1U) ++cnt;
            /* each 1 must have 3 or 4 neighbouring 1s */
            if (cnt != 3u && cnt != 4u) return false;
        }
    }
    return true;
}

/* pre‑computed transitions: for each (above,current) record all
   possible below rows that satisfy the validity rule.  We store
   a 32‑bit mask with bit i set iff row i can be below. */
static uint32_t trans[32][32];
static bool trans_initialized = false;

static void init_trans(void)
{
    if (trans_initialized) return;
    for (uint32_t above = 0; above < 32u; ++above)
        for (uint32_t current = 0; current < 32u; ++current)
        {
            uint32_t mask = 0u;
            for (uint32_t below = 0; below < 32u; ++below)
                if (valid_transition(above, current, below))
                    mask |= (1u << below);
            trans[above][current] = mask;
        }
    trans_initialized = true;
}

/* required external linkage */
seq_t a(idx_t n)
{
    /* guard against type mismatch at compile time */
    (void)sizeof((idx_t)0 + (seq_t)0);  /* dummy to silence unused warnings */

    init_trans();

    if (n == 1u) return (seq_t)1;

    /* DP arrays over two consecutive rows: state index = (above << 5) | current */
    static seq_t dp[1024];
    static seq_t dp_next[1024];

    /* clear dp */
    for (int i = 0; i < 1024; ++i) dp[i] = 0;

    /* initialise with first two rows; virtual top row is all zeros */
    for (uint32_t r1 = 0; r1 < 32u; ++r1)
    {
        uint32_t mask = trans[0][r1];
        for (uint32_t r2 = 0; r2 < 32u; ++r2)
            if (mask & (1u << r2))
                dp[(r1 << 5) | r2] += (seq_t)1;
    }

    /* Process rows 3..n */
    for (idx_t i = 3; i <= n; ++i)
    {
        for (int j = 0; j < 1024; ++j) dp_next[j] = 0;

        for (int idx = 0; idx < 1024; ++idx)
        {
            seq_t cur = dp[idx];
            if (cur == 0) continue;
            uint32_t above   = idx >> 5;
            uint32_t current = idx & 31u;
            uint32_t mask = trans[above][current];
            uint32_t m = mask;
            while (m)
            {
                uint32_t bit = m & -m;
                uint32_t below = __builtin_ctz(m);
                uint32_t next_idx = (current << 5) | below;
                dp_next[next_idx] += cur;
                m ^= bit;
            }
        }

        /* swap dp and dp_next */
        for (int j = 0; j < 1024; ++j) dp[j] = dp_next[j];
    }

    /* bottom validation: last row must satisfy rule with below=0 */
    seq_t ans = 0;
    for (int idx = 0; idx < 1024; ++idx)
    {
        uint32_t above   = idx >> 5;
        uint32_t current = idx & 31u;
        if (valid_transition(above, current, 0u))
            ans += dp[idx];
    }
    return ans;
}
