/*
 * A252286  Number of (n+2)X(1+2) 0..2 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 0 1 or 4.
 * https://oeis.org/A252286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 19, u64 n <= 43, u128 n <= 91,
 *            big any n
 * Helpers:   free_seq_array_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of (n+2) × 3 arrays with entries 0,1,2 such that
   every 3×3 subblock has all its row, column, main diagonal and
   anti‑diagonal sums not equal to 0, 1 or 4.  The value is computed by
   dynamic programming over the last two rows of the array. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/free_seq_array_custom.h"

/* Precomputed data: all possible single rows of length 3 (base‑3
   encodings) and the transition masks between state pairs.  A state
   is an ordered pair (r1,r2) of such rows.  There are 27 rows, 729
   states. */
static int rows[27][3];
static int row_sum[27];
static uint32_t next_mask[729];
static bool tables_ready = false;

/* Initialise the tables once. */
static void init_tables(void)
{
    /* Enumerate all rows. */
    for (int i = 0; i < 27; ++i) {
        int v = i;
        rows[i][0] = v % 3; v /= 3;
        rows[i][1] = v % 3; v /= 3;
        rows[i][2] = v % 3;
        row_sum[i] = rows[i][0] + rows[i][1] + rows[i][2];
    }

    /* For each pair of rows (state) pre‑compute which third rows are
       admissible. */
    for (int s = 0; s < 729; ++s) {
        int r1 = s / 27;
        int r2 = s % 27;
        uint32_t mask = 0U;

        /* If r1 or r2 already violates the row‑sum condition, no
           transition possible from this state. */
        if (row_sum[r1] == 0 || row_sum[r1] == 1 || row_sum[r1] == 4 ||
            row_sum[r2] == 0 || row_sum[r2] == 1 || row_sum[r2] == 4) {
            next_mask[s] = 0U;
            continue;
        }

        /* Try all possible third rows. */
        for (int r3 = 0; r3 < 27; ++r3) {
            if (row_sum[r3] == 0 || row_sum[r3] == 1 || row_sum[r3] == 4)
                continue;

            /* Column sums. */
            int col[3];
            bool ok = true;
            for (int c = 0; c < 3; ++c) {
                col[c] = rows[r1][c] + rows[r2][c] + rows[r3][c];
                if (col[c] == 0 || col[c] == 1 || col[c] == 4) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;

            /* Main diagonal. */
            int dsum = rows[r1][0] + rows[r2][1] + rows[r3][2];
            if (dsum == 0 || dsum == 1 || dsum == 4) continue;

            /* Anti‑diagonal. */
            int adsum = rows[r1][2] + rows[r2][1] + rows[r3][0];
            if (adsum == 0 || adsum == 1 || adsum == 4) continue;

            mask |= 1U << r3;
        }
        next_mask[s] = mask;
    }
    tables_ready = true;
}

/* Allocate an array of seq_t with space for 'cap' elements.  If the
   build provides a helper `seq_alloc`, it will be used; otherwise
   malloc is used directly.  The array is zero‑initialised. */
static seq_t *alloc_seq_array(size_t cap)
{
#ifdef seq_alloc
    seq_t *p = seq_alloc(cap);
#else
    seq_t *p = (seq_t *)malloc(cap * sizeof(seq_t));
    if (p) memset(p, 0, cap * sizeof(seq_t));
#endif
    return p;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!tables_ready) init_tables();

    /* DP arrays: current and next state counts. */
    seq_t *cur = alloc_seq_array(729);
    seq_t *nxt = alloc_seq_array(729);
    if (!cur || !nxt) {
        if (cur) free_seq_array_custom(cur);
        if (nxt) free_seq_array_custom(nxt);
        return 0;                /* out of memory */
    }

    /* Initially, any ordered pair of rows is a possible start (before
       considering any block). */
    for (int i = 0; i < 729; ++i) cur[i] = 1;

    /* For each of the n blocks, apply transitions. */
    for (idx_t step = 0; step < n; ++step) {
        /* Clear nxt.  If the allocation helper does not zero the
           memory, do it explicitly. */
        memset(nxt, 0, 729 * sizeof(seq_t));

        for (int s = 0; s < 729; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            uint32_t mask = next_mask[s];
            if (!mask) continue;
            int r2 = s % 27;
            while (mask) {
                uint32_t lsb = mask & -mask;
                int r3 = __builtin_ctz(mask);
                int t = r2 * 27 + r3;
                nxt[t] += val;
                mask ^= lsb;
            }
        }

        /* Swap cur and nxt for the next iteration. */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all final states. */
    seq_t total = 0;
    for (int i = 0; i < 729; ++i) total += cur[i];

    free_seq_array_custom(cur);
    free_seq_array_custom(nxt);
    return total;
}
