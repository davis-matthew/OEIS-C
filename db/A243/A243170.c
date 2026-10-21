/*
 * A243170  Nonnegative integers of the form 4x^2+xy-4y^2.
 * https://oeis.org/A243170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243170: Nonnegative integers of the form 4x^2 + x*y – 4y^2
 * sorted in increasing order with duplicates removed. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Comparator for qsort. Works for both built‑in integral types and
   arbitrary‑precision integer types because it relies on the operators
   available for seq_t. */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *aa = (const seq_t *)a;
    const seq_t *bb = (const seq_t *)b;
    if (*aa < *bb) return -1;
    if (*aa > *bb) return 1;
    return 0;
}

/* ------------------------------------------------------------------ */
/* Core sequence function.  Assumes 1‑based indexing. */
seq_t a(idx_t n)
{
    /* 0‑based return for invalid offset (behaviour undefined for n==0). */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Start with a modest search radius; double until enough values. */
    size_t limit = 100;

    while (1) {
        /* Maximum number of (x,y) pairs in the square grid [-limit,limit]^2. */
        size_t grid_size = (2 * limit + 1);
        size_t max_pairs = grid_size * grid_size;

        /* Allocate array for all non‑negative values found. */
        seq_t *vals = (seq_t *)malloc(max_pairs * sizeof(seq_t));
        if (!vals) /* allocation failed – give up (should not happen). */
            return (seq_t)0;

        size_t len = 0;

        /* Enumerate all integer pairs (x,y) in the grid. */
        for (int64_t x = -(int64_t)limit; x <= (int64_t)limit; ++x) {
            for (int64_t y = -(int64_t)limit; y <= (int64_t)limit; ++y) {
                int64_t v = 4LL * x * x + x * y - 4LL * y * y;
                if (v >= 0) {
                    vals[len++] = (seq_t)v;
                }
            }
        }

        /* Sort the values so duplicates become adjacent. */
        qsort(vals, len, sizeof(seq_t), cmp_seq);

        /* Remove duplicates in‑place. */
        size_t uniq = 0;
        for (size_t i = 0; i < len; ++i) {
            if (i == 0 || vals[i] != vals[i - 1]) {
                vals[uniq++] = vals[i];
            }
        }

        /* Does the current list contain the requested n‑th element? */
        if (uniq >= n) {
            seq_t result = vals[n - 1];   /* 1‑based → 0‑based index */
            free(vals);
            return result;
        }

        /* Not enough values – enlarge the search grid.  Double the limit. */
        free(vals);
        if (limit > SIZE_MAX / 2) {
            /* Prevent overflow – stop at maximum representable limit. */
            limit = SIZE_MAX / 2;
        } else {
            limit *= 2;
        }
    }
}
