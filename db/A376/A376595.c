/*
 * A376595  Points of nonzero curvature in the sequence of nonsquarefree
 *          numbers (A013929).
 * https://oeis.org/A376595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   ensure_capacity, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376595: a(n) is the n‑th index i (1‑based) in the
   nonsquarefree numbers A013929 for which the second difference
   (A376593) of that sequence is non‑zero.  The function iterates
   over the nonsquarefree numbers, keeps track of successive gaps,
   and records i whenever two consecutive gaps differ. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h" /* provides bool is_squarefree(seq_t x) */
#include "../utils/ensure_capacity.h" /* provides void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size) */

seq_t a(idx_t n) {
    /* static state for incremental computation */
    static seq_t *curvature_indices = NULL;
    static size_t curv_cap = 0;
    static size_t curv_len = 0;

    static uint64_t next_candidate = 2;   /* next integer to test for nonsquarefree */
    static uint64_t nonsq_count = 0;      /* count of nonsquarefree numbers processed */
    static seq_t prev_B = 0;              /* previous nonsquarefree number in B */
    static bool have_prev_gap = false;    /* whether a previous gap exists */
    static seq_t prev_gap = 0;            /* last gap value used */

    /* Ensure the array can hold at least one more element if needed */
    if (curv_cap == 0) {
        size_t init_cap = 16;
        ensure_capacity(init_cap, &curv_cap, (void**)&curvature_indices, sizeof(seq_t));
        curv_len = 0;
    }

    /* Generate nonsquarefree numbers until we have enough curvature indices */
    while (curv_len < n) {
        /* Find the next nonsquarefree integer */
        while (next_candidate < 0xFFFFFFFFFFFFFFFFULL && is_squarefree((seq_t)next_candidate))
            next_candidate++;

        if (next_candidate == 0xFFFFFFFFFFFFFFFFULL) {
            /* Should not happen in the working range */
            break;
        }

        seq_t m = (seq_t)next_candidate;         /* current nonsquarefree number B[nonsq_count] */
        nonsq_count++;

        if (nonsq_count == 1) {
            prev_B = m;                           /* first element, no gap yet */
        } else {
            seq_t gap = m - prev_B;                /* new gap between B[nonsq_count] and B[nonsq_count-1] */

            if (have_prev_gap) {
                seq_t s = gap - prev_gap;         /* second difference s_i */
                if (s != 0) {
                    /* curvature index is nonsq_count - 2 (1‑based) */
                    seq_t idx_val = (seq_t)(nonsq_count - 2);
                    /* Ensure array capacity */
                    if (curv_len == curv_cap) {
                        size_t new_cap = curv_cap * 2;
                        ensure_capacity(new_cap, &curv_cap, (void**)&curvature_indices,
                                        sizeof(seq_t));
                    }
                    curvature_indices[curv_len++] = idx_val;
                }
            }
            prev_gap = gap;
            have_prev_gap = true;
            prev_B = m;
        }
        next_candidate++;                         /* advance to the next integer */
    }

    /* Requested term is stored (n is 1‑based) */
    return curvature_indices[(size_t)(n - 1)];
}
