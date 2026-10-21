/*
 * A250968  Number of (n+1)X(3+1) 0..1 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements less than the absolute difference of
 *          its antidiagonal elements.
 * https://oeis.org/A250968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 binary arrays that contain no 2×2 *
 *            subblock with diagonal‐sum < |antidiagonal‐difference|. * This
 *            is computed by dynamic programming on the 16 possible rows. *
 *            The transition from row i to row j is allowed iff for every *
 *            adjacent column pair k=0,1,2 we do NOT have * (bi...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* The 4-bit row is represented by the lower 4 bits of a 64‑bit integer. */
static unsigned char trans[16][16];
static bool trans_ready = false;

static void build_transition(void)
{
    for (unsigned i = 0; i < 16; ++i) {
        for (unsigned j = 0; j < 16; ++j) {
            bool ok = true;
            for (unsigned k = 0; k < 3 && ok; ++k) {
                unsigned a = (i >> k)       & 1U;
                unsigned b = (i >> (k + 1)) & 1U;
                unsigned c = (j >> k)       & 1U;
                unsigned d = (j >> (k + 1)) & 1U;
                if ((b != c) && (a == 0) && (d == 0))
                    ok = false;
            }
            trans[i][j] = ok ? 1U : 0U;
        }
    }
    trans_ready = true;
}

/* Compute a(n) for n >= 1.  */
seq_t a(idx_t n)
{
    /* zero or negative index is a sentinel; callers use n >= 1. */
    if (n < 1)
        return (seq_t)0;

    if (!trans_ready)
        build_transition();

    seq_t cur[16];
    seq_t next[16];

    /* Initial state: one row can be any of the 16 possibilities. */
    for (unsigned i = 0; i < 16; ++i)
        cur[i] = (seq_t)1;

    /* Perform n transitions to reach (n+1) rows. */
    for (idx_t step = 0; step < n; ++step) {
        for (unsigned j = 0; j < 16; ++j)
            next[j] = (seq_t)0;
        for (unsigned i = 0; i < 16; ++i) {
            seq_t ci = cur[i];
            if (ci == (seq_t)0) continue;
            for (unsigned j = 0; j < 16; ++j)
                if (trans[i][j])
                    next[j] += ci;
        }
        /* Prepare for the next step. */
        for (unsigned j = 0; j < 16; ++j)
            cur[j] = next[j];
    }

    /* Sum all paths ending in any row. */
    seq_t total = (seq_t)0;
    for (unsigned j = 0; j < 16; ++j)
        total += cur[j];

    return total;
}
