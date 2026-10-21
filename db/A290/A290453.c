/*
 * A290453  Triangle formed by reading the triangle of Eulerian numbers
 *          (A173018) mod 3.
 * https://oeis.org/A290453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the Eulerian number A(r,c) modulo 3, where r is the
 *            unique integer with r*(r+1)/2 <= n < (r+1)*(r+2)/2 and c = n -
 *            r*(r+1)/2. The triangle A173018 is read row‑major
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

static seq_t *tri = NULL;          /* flattened triangle values */
static idx_t  max_row = (idx_t)-1;  /* highest computed row */

/* Helper to compute the row index r for a given n */
static idx_t row_of(idx_t n)
{
    idx_t low = 0, high = 1;

    /* find an upper bound for r */
    while (high * (high + 1) / 2 <= n) {
        if (high > ((idx_t)-1) / 2) {
            high = (idx_t)-1;
            break;
        }
        high <<= 1;
    }

    /* binary search */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* determine (row, column) */
    idx_t r = row_of(n);
    idx_t col = n - r * (r + 1) / 2;

    /* extend the triangle if necessary */
    if (tri == NULL || r > max_row) {
        idx_t new_max = r;
        size_t new_cap = (size_t)(new_max + 1) * (new_max + 2) / 2;
        seq_t *new_tri = (seq_t *)malloc(new_cap * sizeof(seq_t));
        size_t old_cap = 0;
        if (tri != NULL) {
            old_cap = (size_t)(max_row + 1) * (max_row + 2) / 2;
            memcpy(new_tri, tri, old_cap * sizeof(seq_t));
            free(tri);
        }
        tri = new_tri;

        /* start generating missing rows */
        idx_t start_row = (max_row == (idx_t)-1) ? 0 : max_row + 1;
        if (max_row == (idx_t)-1) {
            /* row 0 */
            tri[0] = 1;
            max_row = 0;
            start_row = 1;
        }

        for (idx_t rr = start_row; rr <= new_max; ++rr) {
            size_t row_start = (size_t)rr * (rr + 1) / 2;
            size_t prev_start = (size_t)(rr - 1) * rr / 2;
            for (idx_t k = 0; k <= rr; ++k) {
                size_t idx = row_start + k;
                if (k == 0) {
                    tri[idx] = 1;          /* A(r,0) = 1 */
                } else if (k == rr) {
                    tri[idx] = 0;          /* A(r,r) = 0 for r>0 */
                } else {
                    unsigned long long val =
                        (unsigned long long)(rr - k) * (unsigned long long)tri[prev_start + (k - 1)]
                        + (unsigned long long)(k + 1) * (unsigned long long)tri[prev_start + k];
                    tri[idx] = (seq_t)(val % 3);
                }
            }
        }
        max_row = new_max;
    }

    size_t index = (size_t)r * (r + 1) / 2 + col;
    return tri[index];
}
