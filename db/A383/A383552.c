/*
 * A383552  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where A(n,k) is the number of lattice paths from (0,0)
 *          to (n,k) using steps (1,0),(0,1),(2,2).
 * https://oeis.org/A383552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = A(n-1,k) + A(n,k-1) + A(n-2,k-2) (n,k ≥ 0) with A(0,0)
 *            = 1 and A(n,0) = A(0,n) = 1. a(n) is the sequence obtained by
 *            reading the square array A by antidiagonals downwards
 * Range:     u8 n <= 58, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   antidiagonal_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/antidiagonal_of.h"

/* Cached table of values A(i,j).  */
static seq_t *table = NULL;
static idx_t max_side = 0;

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Determine which antidiagonal the index n falls on. */
    idx_t d = antidiagonal_of(n);

    /* Resize the cache if necessary. */
    if (d >= max_side)
    {
        size_t new_side = (size_t)(d + 1);
        /* Allocate a new table of size new_side × new_side. */
        seq_t *new_table =
            (seq_t *)malloc(sizeof(seq_t) * new_side * new_side);
        if (!new_table)
            return 0; /* allocation failure → return 0 as safe value */

        /* Fill the table using the recurrence. */
        for (idx_t i = 0; i <= d; ++i)
        {
            for (idx_t j = 0; j <= d; ++j)
            {
                size_t pos = (size_t)i * new_side + j;
                if (i == 0 || j == 0)
                {
                    new_table[pos] = 1;
                }
                else
                {
                    seq_t val = new_table[(size_t)(i - 1) * new_side + j] +
                                new_table[(size_t)i * new_side + (j - 1)];
                    if (i >= 2 && j >= 2)
                        val += new_table[(size_t)(i - 2) * new_side + (j - 2)];
                    new_table[pos] = val;
                }
            }
        }

        /* Replace the old table. */
        free(table);
        table = new_table;
        max_side = new_side;
    }

    /* Compute the coordinates (i,j) on antidiagonal d. */
    __int128 start128 = (__int128)d * (d + 1) / 2;
    idx_t start = (idx_t)start128;
    idx_t offset = n - start;
    idx_t i_coord = offset;
    idx_t j_coord = d - offset;

    size_t pos = (size_t)i_coord * max_side + j_coord;
    return table[pos];
}
