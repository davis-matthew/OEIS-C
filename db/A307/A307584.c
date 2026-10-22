/*
 * A307584  Triangle read by rows: T(n,k) is the number of non-backtracking
 *          walks on Z^2 of length n that are active for k steps, where the
 *          walk is initially active and turns in the walk toggle the
 *          activity.
 * https://oeis.org/A307584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) is obtained by enumerating non‑backtracking walks on Z² of
 *            length r, where the walk starts active and a turn toggles
 *            activity. The triangle T(r,k) gives the number of such walks
 *            that are active for k steps. The sequence a(n) is the flattened
 *            triangle read by rows
 * Range:     u8 n <= 24, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   twice
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses an iterative DP that tracks current direction
   parity and the cumulative number of active steps.  All DP values are
   stored in the type seq_t.  The code keeps a static cache of
   previously computed terms so that repeated calls are fast.  The
   maximum requested index is assumed to be <= 10000 (row ≤ 150).  */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/twice.h"

/* Maximum index that can be queried.  n <= 10000 ⇒ row <= 150. */
#define MAX_INDEX 20000
#define MAX_ROW   200

/* Cached values of the sequence.  flat[i] holds a(i+1) because the
   sequence is 1‑indexed. */
static seq_t flat[MAX_INDEX];
static size_t flat_len = 0;           /* number of values currently stored */

/* DP arrays for the current row.  even[k] = number of walks of the
   current length that end with parity 0 (activity is active before the
   next step) and have k active steps so far.  odd[k] analogous. */
static seq_t even[MAX_ROW + 2];
static seq_t odd[MAX_ROW + 2];
static size_t cur_row = 0;            /* number of rows already processed */

seq_t a(idx_t n)
{
    /* Initialise at first call.  Compute the first row. */
    if (flat_len == 0) {
        /* Row 1: only one walk of length 1, active throughout. */
        cur_row = 1;
        even[1] = (seq_t)1;
        odd[1]  = (seq_t)0;
        flat[0] = even[1] + odd[1];
        flat_len = 1;
    }

    /* Generate rows until we have at least n terms. */
    while (flat_len < (size_t)n) {
        size_t next_row = cur_row + 1;
        seq_t new_even[MAX_ROW + 2];
        seq_t new_odd [MAX_ROW + 2];
        /* Initialise new arrays to zero.  seq_t may not be a POD,
           but zeroing the bytes results in a zero value for arithmetic
           types and is safe for big‑int classes that treat all‑zero
           as zero. */
        memset(new_even, 0, sizeof(new_even));
        memset(new_odd,  0, sizeof(new_odd));

        for (size_t k = 1; k <= cur_row; ++k) {
            seq_t e = even[k];
            seq_t o = odd[k];

            if (e != (seq_t)0) {
                /* Straight: activity stays, so current step is active. */
                new_even[k + 1] += e;
                /* Turn: parity toggles; step i+1 is inactive. */
                new_odd[k] += twice(e);
            }
            if (o != (seq_t)0) {
                /* Straight from odd parity: step inactive. */
                new_odd[k] += o;
                /* Turn from odd parity toggles to even; step active. */
                new_even[k + 1] += twice(o);
            }
        }

        /* Commit new DP arrays into even/odd for the next iteration. */
        for (size_t k = 1; k <= next_row; ++k) {
            even[k] = new_even[k];
            odd[k]  = new_odd[k];
        }

        cur_row = next_row;

        /* Append the values of the new row to the flattened sequence. */
        for (size_t k = 1; k <= cur_row; ++k) {
            if (flat_len >= MAX_INDEX) {
                /* In practice this should never happen for the
                   constraints of the problem. */
                return (seq_t)0;
            }
            flat[flat_len++] = even[k] + odd[k];
        }
    }

    return flat[n - 1];
}
