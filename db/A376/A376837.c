/*
 * A376837  a(n) is the number of paths to reach a position outside an 8 X 8
 *          chessboard after n steps, starting in one of the corners, when
 *          performing a walk with unit steps on the square lattice.
 * https://oeis.org/A376837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A376837.
   Recurrence: a(n) = number of length‑n rook walks that exit an
   8×8 board when starting at a corner.  The values are obtained with
   dynamic programming over all board positions.  The routine memoises
   all computed terms so that arbitrary order of calls is supported. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage for memoisation */
    static seq_t *answers      = NULL;   /* answers[0] unused, 1‑based */
    static size_t answers_cap  = 0;      /* allocated size of answers */
    static idx_t  max_computed = 0;      /* largest n already computed */

    /* Permanent 8×8 board state arrays.  Cur holds counts for the
       current step; next will accumulate counts for the next step. */
    static seq_t board_cur[64];
    static seq_t board_next[64];
    static seq_t *cur  = board_cur;
    static seq_t *next = board_next;

    /* ---------- initialise static data --------------------------------------------------- */
    if (answers == NULL) {
        /* Allocate minimal storage (size 1 for index 0). */
        answers = (seq_t *)malloc((size_t)1 * sizeof(seq_t));
        if (answers == NULL) return 0;           /* allocation failure */
        answers_cap = 1;
        answers[0] = 0;

        /* Initialise board state: start from corner (0,0). */
        for (size_t i = 0; i < 64; ++i) cur[i] = (seq_t)0;
        cur[0] = (seq_t)1;
        /* board_next is already zero because it has static storage */
    }

    /* ---------- extend computed values up to requested n ------------------------------- */
    if (n > max_computed) {
        /* Grow answers array if necessary. */
        if ((size_t)n + 1 > answers_cap) {
            seq_t *tmp = (seq_t *)realloc(answers, ((size_t)n + 1) * sizeof(seq_t));
            if (tmp == NULL) return 0;          /* allocation failure */
            answers = tmp;
            answers_cap = (size_t)n + 1;
        }

        /* Compute successive steps from max_computed+1 to n. */
        for (idx_t step = max_computed + 1; step <= n; ++step) {
            /* clear next array */
            for (size_t i = 0; i < 64; ++i) next[i] = (seq_t)0;

            seq_t exits = (seq_t)0;

            /* iterate over all board cells */
            for (idx_t r = 0; r < 8; ++r) {
                for (idx_t c = 0; c < 8; ++c) {
                    size_t idx = (size_t)(r * 8 + c);
                    seq_t ways = cur[idx];
                    if (ways == (seq_t)0) continue;

                    /* Up */
                    if (r < 7) next[(size_t)((r + 1) * 8 + c)] += ways;
                    else      exits += ways;

                    /* Down */
                    if (r > 0) next[(size_t)((r - 1) * 8 + c)] += ways;
                    else      exits += ways;

                    /* Right */
                    if (c < 7) next[(size_t)(r * 8 + (c + 1))] += ways;
                    else      exits += ways;

                    /* Left */
                    if (c > 0) next[(size_t)(r * 8 + (c - 1))] += ways;
                    else      exits += ways;
                }
            }

            /* Store the result for this step and prepare for next iteration. */
            answers[(size_t)step] = exits;

            /* swap cur and next for next iteration */
            seq_t *tmp_ptr = cur;
            cur = next;
            next = tmp_ptr;
        }

        max_computed = n;
    }

    return answers[(size_t)n];
}
