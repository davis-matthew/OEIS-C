/*
 * A258180  Sum over all Dyck paths of semilength n of products over all peaks
 *          p of C(x_p,y_p), where x_p and y_p are the coordinates of peak p.
 * https://oeis.org/A258180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{P Dyck, semilength n} ∏_{p∈peaks(P)} C(x_p , y_p)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(x,y) = binom(x,y) and (x_p , y_p) are the coordinates of each peak p.  */

#include <stdlib.h>
#include "../utils/binom.h"  /* provides seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition (empty path, no peaks). */
    if (n == 0)
        return (seq_t)1;

    const size_t sz = (size_t)n + 1;               /* maximum possible height */

    /* Allocate DP tables. Cast for C++ compliance when seq_t is a class. */
    seq_t *dpU   = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *dpD   = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *newU  = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *newD  = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!dpU || !dpD || !newU || !newD) {
        /* Allocation failed – normally impossible for tested sizes. */
        free(dpU); free(dpD); free(newU); free(newD);
        return (seq_t)0;
    }

    /* The starting state: at height 0, with a virtual previous step of type U,
       so that the first real step may be an up-step. */
    dpU[0] = (seq_t)1;

    const idx_t steps = 2 * n;    /* total number of steps in a Dyck path */

    for (idx_t step = 1; step <= steps; ++step) {
        /* After 'step' steps the height cannot exceed certain bounds. */
        const idx_t max_h = (step <= n) ? step : (2 * n - step);

        /* Clear new tables up to max_h. */
        memset(newU, 0, (max_h + 1) * sizeof(seq_t));
        memset(newD, 0, (max_h + 1) * sizeof(seq_t));

        /* Transition to an up-step: from height h-1 to h. */
        for (idx_t h = 1; h <= max_h; ++h)
            newU[h] = dpU[h - 1] + dpD[h - 1];

        /* Transition to a down-step: from previous height 'prev' to 'prev-1'. */
        const idx_t max_prev = (step - 1 <= n) ? (step - 1) : n;
        for (idx_t prev = 1; prev <= max_prev; ++prev) {
            seq_t no_peak   = dpD[prev];   /* path ended with a down, no new peak */
            seq_t peak_part = dpU[prev];   /* path ended with an up → peak */
            if (peak_part)
                peak_part *= binom(step - 1, prev);  /* product over this peak */

            newD[prev - 1] = newD[prev - 1] + no_peak + peak_part;
        }

        /* Swap current and next tables to proceed to the next step. */
        seq_t *tmp;
        tmp = dpU; dpU = newU; newU = tmp;
        tmp = dpD; dpD = newD; newD = tmp;
    }

    /* After all steps, only paths ending at height 0 with a final down-step are valid. */
    seq_t result = dpD[0];

    /* Free allocated memory. */
    free(dpU); free(dpD); free(newU); free(newD);

    return result;
}
