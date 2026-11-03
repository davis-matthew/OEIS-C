/*
 * A335132  Numbers whose binary expansion generates 3-fold rotationally
 *          symmetric EQ-triangles.
 * https://oeis.org/A335132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   term is the nth number whose binary expansion generates a 3‑fold
 *            rotationally symmetric EQ‑triangle. Computation is done by a
 *            membership test and linear search
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Test whether a given number m generates a triangle with
   3‑fold rotational symmetry. 0 is accepted by definition. */
static bool is_member(seq_t m) {
    if (m == 0) {
        return true;
    }

    /* Determine the bit length L of m */
    seq_t tmp = m;
    idx_t L = 0;
    while (tmp != 0) {
        ++L;
        tmp >>= 1;
    }

    /* Compute row offsets for indexing into the linear triangle buffer */
    idx_t *row_off = (idx_t *)malloc((size_t)L * sizeof(idx_t));
    row_off[0] = 0;
    for (idx_t r = 1; r < L; ++r) {
        row_off[r] = row_off[r - 1] + (L - (r - 1));
    }

    /* Total number of cells in the triangle */
    size_t total_cells = (size_t)L * (L + 1) / 2;
    uint8_t *tri = (uint8_t *)malloc(total_cells);
    if (!row_off || !tri) {
        /* In a real build, we would handle allocation failure. */
        return false;
    }

    /* Build the triangle row by row */
    /* First row (row 0) is the binary expansion of m. */
    idx_t start0 = row_off[0];
    for (idx_t j = 0; j < L; ++j) {
        tri[start0 + j] = (uint8_t)((m >> (L - 1 - j)) & 1);
    }

    /* Subsequent rows */
    for (idx_t r = 1; r < L; ++r) {
        idx_t len_curr = L - r;
        idx_t start_curr = row_off[r];
        idx_t start_prev = row_off[r - 1];
        for (idx_t c = 0; c < len_curr; ++c) {
            tri[start_curr + c] =
                (tri[start_prev + c] == tri[start_prev + c + 1]) ? 1 : 0;
        }
    }

    /* Check 3‑fold rotational symmetry */
    for (idx_t i = 0; i < L; ++i) {
        for (idx_t j = 0; j + i < L; ++j) {
            idx_t idx = row_off[i] + j;
            idx_t ip = L - 1 - i - j;
            idx_t jp = i;
            idx_t idxp = row_off[ip] + jp;
            if (tri[idx] != tri[idxp]) {
                free(row_off);
                free(tri);
                return false;  /* symmetry fails */
            }
        }
    }

    free(row_off);
    free(tri);
    return true;  /* all symmetry checks passed */
}

/* Forward declaration of the entry routine */
static bool is_member(seq_t m);

/* Return the n-th term of A335132 (1‑based index). */
seq_t a(idx_t n) {
    if (n == 1) {
        return 0;                /* a(1) = 0 */
    }
    idx_t found = 1;             /* count including 0 */
    seq_t candidate = 1;         /* first odd number */

    while (found < n) {
        if (is_member(candidate)) {
            ++found;
            if (found == n) {
                return candidate;
            }
        }
        candidate += 2;         /* skip even numbers */
    }
    /* Should never reach here */
    return 0;
}
