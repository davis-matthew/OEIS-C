/*
 * A329665  Number of meanders of length n with Motzkin-steps avoiding the
 *          consecutive steps UD, HH and DU.
 * https://oeis.org/A329665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation method: dynamic programming over the state
   (altitude, last step).  The recurrence follows from the
   forbidden consecutive patterns UD, HH and DU and the
   Motzkin step set U=(1,1), H=(1,0), D=(1,-1).
   Initial state: start with no previous step.  The DP
   counts meanders of length n staying at nonnegative
   altitude.  Complexity: O(n^2); fits easily in the limits. */

#include <stddef.h>
#include <stdlib.h>

/* returns the nth term of OEIS A329665 */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* arrays are indexed by altitude: 0 .. n */
    size_t N = (size_t)n + 1;
    seq_t *U_cur = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *H_cur = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *D_cur = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *U_next = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *H_next = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *D_next = (seq_t *)calloc(N, sizeof(seq_t));

    /* initial step (length 1) */
    U_cur[1] = 1;
    H_cur[0] = 1;

    for (size_t step = 1; step < (size_t)n; ++step) {
        /* clear next arrays up to step+1 (max altitude after this step) */
        for (size_t i = 0; i <= step + 1; ++i) {
            U_next[i] = 0;
            H_next[i] = 0;
            D_next[i] = 0;
        }

        for (size_t h = 0; h <= step; ++h) {
            seq_t valU = U_cur[h];
            if (valU) {
                U_next[h + 1] += valU;   /* U -> U */
                H_next[h]     += valU;   /* U -> H */
            }

            seq_t valH = H_cur[h];
            if (valH) {
                U_next[h + 1] += valH;   /* H -> U */
                if (h > 0) {
                    D_next[h - 1] += valH;   /* H -> D */
                }
            }

            seq_t valD = D_cur[h];
            if (valD) {
                H_next[h]     += valD;   /* D -> H */
                if (h > 0) {
                    D_next[h - 1] += valD;   /* D -> D */
                }
            }
        }

        /* swap current and next arrays */
        seq_t *tmp;
        tmp   = U_cur;   U_cur   = U_next;   U_next   = tmp;
        tmp   = H_cur;   H_cur   = H_next;   H_next   = tmp;
        tmp   = D_cur;   D_cur   = D_next;   D_next   = tmp;
    }

    seq_t sum = 0;
    for (size_t h = 0; h <= (size_t)n; ++h) {
        sum += U_cur[h] + H_cur[h] + D_cur[h];
    }

    /* free allocated memory */
    free(U_cur); free(H_cur); free(D_cur);
    free(U_next); free(H_next); free(D_next);

    return sum;
}
