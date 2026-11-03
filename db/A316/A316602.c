/*
 * A316602  Positive determinant values assumed by performing all permutations
 *          of entries in the 3 X 3 matrix of A316601.
 * https://oeis.org/A316602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth positive determinant obtainable by permuting the
 *            entries of the fixed 3×3 matrix from A316601. The matrix is
 *            fixed as [0, 3, 19, 94, 88, 93, 83, 75, 65]. We generate all 9!
 *            permutations of the 9 entries, compute the determinant of each
 *            permutation, collect the distinct positive det...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Validated: full, pass, 5040 terms through n = 5040 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Comparator for qsort on seq_t */
static int comp_seq_t(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Recursive Heap's algorithm to generate all permutations of arr[9].
   For each permutation we compute the 3×3 determinant and, if it
   is positive, store it in out[ptr].  */
static void heap_permute(idx_t k, seq_t *arr,
                         seq_t *out, size_t *ptr, size_t cap)
{
    if (k == 1) {
        /* Compute determinant of the current permutation. */
        seq_t a = arr[0], b = arr[1], c = arr[2];
        seq_t d = arr[3], e = arr[4], f = arr[5];
        seq_t g = arr[6], h = arr[7], i = arr[8];

        seq_t det = a * (e * i - f * h)
                  - b * (d * i - f * g)
                  + c * (d * h - e * g);

        if (det > 0) {
            out[*ptr] = det;
            ++(*ptr);
            /* Cap is large enough for all permutations (9! = 362880). No
               resize needed. */
        }
        return;
    }

    heap_permute(k - 1, arr, out, ptr, cap);

    for (idx_t i = 0; i < k - 1; ++i) {
        if (k % 2 == 0)
            /* Swap arr[i] and arr[k-1] */
            {
            seq_t tmp = arr[i];
            arr[i] = arr[k - 1];
            arr[k - 1] = tmp;
            }
        else
            {
            seq_t tmp = arr[0];
            arr[0] = arr[k - 1];
            arr[k - 1] = tmp;
            }

        heap_permute(k - 1, arr, out, ptr, cap);
    }
}

/* Main function to compute the nth term. */
seq_t a(idx_t n)
{
    /* Static storage for the computed dataset. */
    static bool initialized = false;
    static seq_t *det_vals = NULL;
    static size_t det_cnt = 0;

    if (!initialized) {
        /* 9! permutations. */
        const idx_t N = 9;
        const idx_t FACT9 = 362880; /* 9! */
        seq_t base[9] = {0, 3, 19, 94, 88, 93, 83, 75, 65};

        /* Allocate temporary array to hold all positive determinants. */
        det_vals = mem_new(seq_t, FACT9);
        if (!det_vals) {
            /* Allocation failed; sequence cannot be computed. */
            return 0;
        }

        /* Temporary array for permutation generation. */
        seq_t perm[9];
        for (idx_t i = 0; i < N; ++i)
            perm[i] = base[i];

        size_t temp_count = 0;
        heap_permute(N, perm, det_vals, &temp_count, FACT9);

        /* Now sort and unique the set. */
        mem_qsort(det_vals, temp_count, sizeof(seq_t), comp_seq_t);

        /* Count distinct values. */
        size_t uniq_count = 0;
        if (temp_count > 0) {
            uniq_count = 1;
            for (size_t i = 1; i < temp_count; ++i) {
                if (det_vals[i] != det_vals[i - 1]) {
                    det_vals[uniq_count] = det_vals[i];
                    ++uniq_count;
                }
            }
        }

        det_cnt = uniq_count;
        initialized = true;
    }

    if (n == 0 || n > det_cnt)
        return 0;
    return det_vals[n - 1];
}
