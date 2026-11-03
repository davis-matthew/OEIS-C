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
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-03)
 * Helpers:   is_squarefree, mem
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
#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

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
        ensure_capacity(init_cap, &curv_cap, OEISC_VPP(&curvature_indices), sizeof(seq_t));
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
                        ensure_capacity(new_cap, &curv_cap, OEISC_VPP(&curvature_indices),
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
