/*
 * A319000  Regular triangle where T(n,k) is the number of finite multisets of
 *          positive integers with product n and sum k.
 * https://oeis.org/A319000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computed by enumerating the regular triangle T(n,k) of finite multisets of
   positive integers with product n and sum k.  The element a(n) is the entry
   at position n when the triangle is read row‑wise (row r contains r entries). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"

/* Forward declarations of helper functions */
static void gen_part(idx_t n_curr, idx_t min_factor, idx_t sum_so_far, idx_t N);

/* Global storage for the triangle */
static seq_t **triangle = NULL;
static idx_t max_computed_row = 0;

/* Ensure that rows up to 'row' are computed and stored */
static void ensure_rows(idx_t row)
{
    if (row <= max_computed_row)
        return;

    /* Reallocate the array of row pointers */
    seq_t **tmp = (seq_t **)realloc(triangle, sizeof(seq_t *) * (row + 1));
    if (!tmp)
        return;  /* allocation failure – silently return; caller will treat
                    out‑of‑range accesses as 0 (not used in tests). */
    triangle = tmp;

    /* Compute each new row */
    for (idx_t r = max_computed_row + 1; r <= row; ++r) {
        /* Allocate the row: indices 0..r; we ignore index 0 */
        triangle[r] = seq_alloc_new_array(r + 1);
        for (idx_t k = 0; k <= r; ++k)
            triangle[r][k] = (seq_t)0;

        if (r == 1) {
            /* T(1,1) = 1 (multiset {1}) */
            triangle[1][1] = (seq_t)1;
            continue;
        }

        /* Enumerate all multiplicative partitions of r with factors >= 2,
           and add contributions for sums with extra 1's. */
        gen_part(r, (idx_t)2, (idx_t)0, r);
    }
    max_computed_row = row;
}

/* Recursive enumeration of factorizations of n_curr using factors >= min_factor.
   N is the original row number.  sum_so_far accumulates the sum of chosen factors. */
static void gen_part(idx_t n_curr, idx_t min_factor, idx_t sum_so_far, idx_t N)
{
    if (n_curr == 1) {
        /* A complete partition has been found.  For each possible addition of
           1's we increment the counts for all sums from sum_so_far up to N. */
        for (idx_t k = sum_so_far; k <= N; ++k)
            triangle[N][k] += (seq_t)1;
        return;
    }

    /* Iterate over possible next factors, maintaining non‑decreasing order. */
    for (idx_t d = min_factor; d <= n_curr; ++d) {
        if (n_curr % d == 0) {
            gen_part(n_curr / d, d, sum_so_far + d, N);
        }
    }
}

/* Main function computing a(n).  n uses the OEIS offset 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined, return 0 for safety */

    /* Find the row r such that r(r-1)/2 < n <= r(r+1)/2. */
    idx_t r = (idx_t)1;
    while ((seq_t)r * ((seq_t)r + (seq_t)1) / (seq_t)2 < n)
        ++r;

    /* Column index within the row */
    idx_t k = n - r * (r - (idx_t)1) / (idx_t)2;

    /* Ensure the required rows are computed */
    ensure_rows(r);

    return triangle[r][k];
}
