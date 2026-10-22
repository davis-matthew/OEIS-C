/*
 * A359662  Number of (3-dimensional) cells of regular m-polytopes for m >= 3.
 * https://oeis.org/A359662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth smallest distinct value among { n*(n+1)/2
 *            (triangular numbers, m>=3), 2^(n-3)*C(n,3) (cube cells, n>=4),
 *            16*C(n,4) (crosspolytope cells, n>=4) } together with the
 *            constants {24,120,600}. The value 1 (from the 3‑dimensional
 *            regular polytopes) is included automatically
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   cross_value, cube_value, max_seq_value128, simplex_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cross_value.h"
#include "../utils/cube_value.h"
#include "../utils/max_seq_value128.h"
#include "../utils/simplex_value.h"

/* The sequence function */
seq_t a(idx_t n)
{
    /* static state across calls */
    static bool initialized = false;

    /* generator indices */
    static idx_t dim_simp;   /* m for simplex, start at 3 */
    static idx_t dim_cube;   /* m for cube,    start at 4 */
    static idx_t dim_cross;  /* m for cross,   start at 4 */

    /* constant values list {24,120,600} */
    static const uint64_t const_vals[3] = {24, 120, 600};
    static idx_t const_idx;            /* index in const_vals */
    static seq_t const_val;            /* current constant */
    static bool const_valid;           /* is const_val <= max seq_t? */

    /* current values of each generator */
    static seq_t val_simp, val_cube, val_cross;
    static bool val_simp_valid, val_cube_valid, val_cross_valid;

    /* cache of generated sequence */
    static seq_t *seq = NULL;
    static size_t len = 0, cap = 0;
    static seq_t last_val = 0;
    static bool has_last = false;

    /* initialise once */
    if (!initialized) {
        dim_simp  = 3;
        dim_cube  = 4;
        dim_cross = 4;
        const_idx = 0;

        /* precompute maximum representable value */
        uint64_t max_val64 = (uint64_t)max_seq_value128();

        /* initialise constant generator, skipping those too large */
        while (const_idx < 3 && const_vals[const_idx] > max_val64)
            ++const_idx;
        if (const_idx < 3) {
            const_val  = (seq_t)const_vals[const_idx];
            const_valid = true;
        } else {
            const_valid = false;
        }

        /* compute initial values of the three sequences */
        val_simp        = simplex_value(dim_simp, &val_simp_valid);
        val_cube        = cube_value(dim_cube, &val_cube_valid);
        val_cross       = cross_value(dim_cross, &val_cross_valid);

        initialized = true;
    }

    /* generate sequence terms up to n */
    while (len < n) {
        /* find the smallest available value */
        seq_t best = 0;
        bool best_set = false;

        if (val_simp_valid && (!best_set || val_simp < best)) { best = val_simp; best_set = true; }
        if (val_cube_valid && (!best_set || val_cube < best)) { best = val_cube; best_set = true; }
        if (val_cross_valid && (!best_set || val_cross < best)) { best = val_cross; best_set = true; }
        if (const_valid && (!best_set || const_val < best)) { best = const_val; best_set = true; }

        /* store if not duplicate */
        if (!has_last || best != last_val) {
            if (len == cap) {
                cap = (cap == 0) ? 16 : cap * 2;
                seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
            }
            seq[len++] = best;
            last_val = best;
            has_last = true;
        }

        /* advance generators that produced the minimum */
        if (val_simp_valid && val_simp == best) {
            ++dim_simp;
            val_simp = simplex_value(dim_simp, &val_simp_valid);
        }
        if (val_cube_valid && val_cube == best) {
            ++dim_cube;
            val_cube = cube_value(dim_cube, &val_cube_valid);
        }
        if (val_cross_valid && val_cross == best) {
            ++dim_cross;
            val_cross = cross_value(dim_cross, &val_cross_valid);
        }
        if (const_valid && const_val == best) {
            ++const_idx;
            uint64_t max_val64 = (uint64_t)max_seq_value128();
            while (const_idx < 3 && const_vals[const_idx] > max_val64)
                ++const_idx;
            if (const_idx < 3) {
                const_val = (seq_t)const_vals[const_idx];
                const_valid = true;
            } else {
                const_valid = false;
            }
        }
    }

    return seq[n - 1];
}
