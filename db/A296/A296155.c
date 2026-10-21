/*
 * A296155  Number of n X 6 0..1 arrays with each 1 adjacent to 3 or 5
 *          king-move neighboring 1's.
 * https://oeis.org/A296155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A296155
 * Number of n X 6 0..1 arrays with each 1 adjacent to 3 or 5 king‑move neighboring 1's.
 * The algorithm builds the set of all 6‑bit row patterns and uses a dynamic
 * programming state of two consecutive rows.  For each triple of consecutive
 * rows we pre‑compute whether the middle one satisfies the adjacency rule.
 * Then we propagate counts row by row.  The final count is the sum of
 * all states whose last row, when followed by a zero row, satisfies the rule. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static bool valid_triple[64][64][64];
static unsigned char next_masks[64 * 64][64];
static unsigned char next_counts[64 * 64];
static bool initialized = false;

/* Pre‑compute validity of all triples and lists of admissible next rows. */
static void init_precomp(void)
{
    if (initialized) return;
    for (unsigned int p = 0; p < 64; p++) {
        for (unsigned int t = 0; t < 64; t++) {
            for (unsigned int n = 0; n < 64; n++) {
                bool ok = true;
                for (int j = 0; j < 6 && ok; j++) {
                    if (t & (1u << j)) {
                        unsigned int cnt = 0;
                        /* horizontal neighbours */
                        if (j > 0 && (t & (1u << (j - 1)))) cnt++;
                        if (j < 5 && (t & (1u << (j + 1)))) cnt++;
                        /* upper neighbours */
                        if (p & (1u << j)) cnt++;                     /* up */
                        if (j > 0 && (p & (1u << (j - 1)))) cnt++;    /* up‑left */
                        if (j < 5 && (p & (1u << (j + 1)))) cnt++;    /* up‑right */
                        /* lower neighbours */
                        if (n & (1u << j)) cnt++;                     /* down */
                        if (j > 0 && (n & (1u << (j - 1)))) cnt++;    /* down‑left */
                        if (j < 5 && (n & (1u << (j + 1)))) cnt++;    /* down‑right */
                        if (cnt != 3 && cnt != 5) { ok = false; break; }
                    }
                }
                valid_triple[p][t][n] = ok;
            }
        }
    }

    /* Build transition lists */
    for (unsigned int p = 0; p < 64; p++) {
        for (unsigned int t = 0; t < 64; t++) {
            unsigned int idx = p * 64 + t;
            unsigned int cnt = 0;
            for (unsigned int n = 0; n < 64; n++) {
                if (valid_triple[p][t][n]) {
                    next_masks[idx][cnt++] = (unsigned char)n;
                }
            }
            next_counts[idx] = (unsigned char)cnt;
        }
    }
    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_precomp();

    /* dp[state] where state = prevprev*64 + prev, counts ways for processed rows */
    seq_t dp[64 * 64] = {0};
    seq_t next[64 * 64] = {0};

    /* Initial state after first row (row 0). prevprev=0, prev=row0 */
    for (unsigned int mask = 0; mask < 64; mask++) {
        dp[mask] = 1;  /* state index = 0*64 + mask */
    }

    /* Process rows 1 .. n-1 */
    for (idx_t row = 1; row < n; ++row) {
        /* clear next */
        for (unsigned int i = 0; i < 64 * 64; ++i) next[i] = 0;

        for (unsigned int src = 0; src < 64 * 64; ++src) {
            seq_t val = dp[src];
            if (!val) continue;
            unsigned char cnt = next_counts[src];
            unsigned char *masks = next_masks[src];
            for (unsigned int k = 0; k < cnt; ++k) {
                unsigned int nmask = masks[k];
                unsigned int dst = ((src % 64) << 6) | nmask;  /* new state: (prev, cur) */
                next[dst] += val;
            }
        }

        /* swap dp and next */
        for (unsigned int i = 0; i < 64 * 64; ++i) dp[i] = next[i];
    }

    /* Final count: sum over all states where last row satisfies rule with next=0 */
    seq_t result = 0;
    for (unsigned int p = 0; p < 64; ++p) {
        for (unsigned int t = 0; t < 64; ++t) {
            unsigned int idx = p * 64 + t;
            if (dp[idx] && valid_triple[p][t][0]) {
                result += dp[idx];
            }
        }
    }

    return result;
}
