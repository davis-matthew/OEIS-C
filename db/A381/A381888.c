/*
 * A381888  Triangle read by rows: T(n, k) = (n + 1) * Sum_{j=k..n}
 *          binomial(n, j) * Eulerian1(j, j - k).
 * https://oeis.org/A381888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sequence A381888 (flattened OEIS triangle)
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial_row.h"

/* Compute floor(sqrt(x)) for 64‑bit values. */
static inline idx_t isqrt_u64(idx_t x) {
    idx_t r = 0;
    /* Binary search on the high bit position. */
    for (int b = 63; b >= 0; --b) {
        idx_t bit = (idx_t)1 << b;
        idx_t cand = r | bit;
        /* Use 128‑bit arithmetic to avoid overflow. */
        if ((__uint128_t)cand * cand <= x)
            r = cand;
    }
    return r;
}

/* Dynamic storage of Eulerian numbers (row 0 up to current max). */
static seq_t **euler = NULL;
static idx_t euler_max = 0;

/* Ensure Eulerian numbers are available up to degree n. */
static void ensure_euler(idx_t n) {
    if (n < euler_max)
        return;

    /* Allocate or grow the outer array. */
    if (euler == NULL) {
        euler = (seq_t**)malloc((n + 1) * sizeof(seq_t*));
        euler[0] = (seq_t*)malloc(sizeof(seq_t));
        euler[0][0] = (seq_t)1;
        euler_max = 0;
    } else if (euler_max < n) {
        seq_t **tmp = (seq_t**)realloc(euler, (n + 1) * sizeof(seq_t*));
        if (tmp) euler = tmp;
        else return; /* Allocation failed – unlikely for testing. */
    }

    /* Compute new rows incrementally. */
    for (idx_t i = euler_max + 1; i <= n; ++i) {
        euler[i] = (seq_t*)calloc((size_t)(i + 1), sizeof(seq_t));
        for (idx_t m = 0; m < i; ++m) {
            seq_t val = 0;
            if (m > 0)
                val += (seq_t)(i - m) * euler[i - 1][m - 1];
            val += (seq_t)(m + 1) * euler[i - 1][m];
            euler[i][m] = val;
        }
        /* Eulerian number with m == i is 0 by convention. */
        euler[i][i] = (seq_t)0;
    }
    euler_max = n;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Determine the row and column for the flattened index n. */
    idx_t r = isqrt_u64(8 * n + 1);
    r = (r - 1) / 2;                     /* row number */
    idx_t prev_count = r * (r + 1) / 2;   /* number of elements before this row */
    idx_t col = n - prev_count;           /* column within the row */

    /* Ensure Eulerian numbers up to row r are ready. */
    ensure_euler(r);

    /* Compute binomial coefficients for this row. */
    seq_t *bin = (seq_t*)malloc((size_t)(r + 1) * sizeof(seq_t));
    binomial_row(r, bin);

    /* Accumulate the sum over j = col..r. */
    seq_t sum = (seq_t)0;
    for (idx_t j = col; j <= r; ++j) {
        seq_t eul = euler[j][j - col];
        sum += bin[j] * eul;
    }

    seq_t result = (seq_t)(r + 1) * sum;

    free(bin);
    return result;
}
