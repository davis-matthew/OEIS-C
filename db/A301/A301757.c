/*
 * A301757  Positive determinant values assumed by performing all permutations
 *          of entries in the 3 X 3 matrix of A301372.
 * https://oeis.org/A301757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 5040, u64 n <= 5040, u128 n <= 5040,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence computed by enumerating all 9! permutations of the known
   optimal 3 × 3 matrix entries (A301372), collecting all positive
   determinants, sorting them and removing duplicates.
   a(1) is the smallest positive determinant, a(2) the next, etc. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

/* comparison function for qsort, works for builtin integers and for
   arbitrary‑precision integer types with operator< and operator>. */
static int seq_t_cmp(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

/* ensure that arr has capacity for at least needed elements */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        assert(tmp);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* next_permutation for an array of uint8_t indices (0‑based) */
static bool next_permutation(uint8_t *a, size_t n)
{
    if (n <= 1) return false;
    int i = (int)n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) i--;
    if (i < 0) return false;
    int j = (int)n - 1;
    while (a[j] <= a[i]) j--;
    uint8_t tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    int l = i + 1, r = (int)n - 1;
    while (l < r) { tmp = a[l]; a[l] = a[r]; a[r] = tmp; l++; r--; }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* static table of distinct positive determinants */
    static seq_t *table = NULL;
    static size_t tblsize = 0;
    static bool initialized = false;

    if (!initialized) {
        /* optimal matrix entries (OEIS A301372) */
        const seq_t vals[9] = { (seq_t)0, (seq_t)1, (seq_t)89,
                                (seq_t)87, (seq_t)99, (seq_t)97,
                                (seq_t)54, (seq_t)61, (seq_t)20 };

        /* iterate over all permutations of indices 0..8 */
        uint8_t perm[9];
        for (size_t i = 0; i < 9; ++i) perm[i] = (uint8_t)i;

        seq_t *det_arr = NULL;
        size_t det_cnt = 0, det_cap = 0;

        do {
            /* determinant of the permuted 3×3 matrix */
            const seq_t a00 = vals[perm[0]], a01 = vals[perm[1]], a02 = vals[perm[2]];
            const seq_t a10 = vals[perm[3]], a11 = vals[perm[4]], a12 = vals[perm[5]];
            const seq_t a20 = vals[perm[6]], a21 = vals[perm[7]], a22 = vals[perm[8]];

            seq_t det = a00 * a11 * a22 + a01 * a12 * a20 + a02 * a10 * a21
                      - a02 * a11 * a20 - a01 * a10 * a22 - a00 * a12 * a21;

            if (det > (seq_t)0) {
                ensure_capacity(&det_arr, &det_cap, det_cnt + 1);
                det_arr[det_cnt++] = det;
            }
        } while (next_permutation(perm, 9));

        /* sort determinants */
        if (det_cnt > 1) qsort(det_arr, det_cnt, sizeof(seq_t), seq_t_cmp);

        /* deduplicate */
        size_t uniq_cnt = 0;
        if (det_cnt > 0) {
            uniq_cnt = 1;
            for (size_t i = 1; i < det_cnt; ++i) {
                if (det_arr[i] != det_arr[uniq_cnt - 1]) {
                    det_arr[uniq_cnt++] = det_arr[i];
                }
            }
        }

        /* shrink to exact size */
        if (uniq_cnt > 0) {
            seq_t *tmp = (seq_t *)realloc(det_arr, uniq_cnt * sizeof(seq_t));
            assert(tmp);
            det_arr = tmp;
        } else {
            free(det_arr);
            det_arr = NULL;
        }

        table   = det_arr;
        tblsize = uniq_cnt;
        initialized = true;
    }

    if (n <= 0) return (seq_t)0;
    if ((idx_t)tblsize < n) return (seq_t)0;
    return table[(size_t)(n - 1)];
}
