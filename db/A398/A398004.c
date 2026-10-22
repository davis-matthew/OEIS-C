/*
 * A398004  Recursively defined symmetric multiplication table read by
 *          antidiagonals: A(n,k), n>=1, k>=1.
 * https://oeis.org/A398004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 142, u32 n <= 543, u64 n <= 990,
 *            u128 n <= 990, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence for A398004 (symmetric multiplication table)
 *
 *  The triangle A(i,j) (i,j >= 1) is defined by
 *          A(1,j) = j
 *          A(i,1) = i
 *          A(i,j) = A(i-1,j) + A(i,j-1) + (i + j)   for i,j >= 2
 *
 *  The sequence a(n) is obtained by reading the triangle by antidiagonals:
 *      a(n) = A(i,j) where
 *          i  = offset on the antidiagonal (starting at 1)
 *          j  = s - i
 *          s  is the unique integer with s(s-1)/2 >= n
 *          offset = n - (s-1)(s-2)/2
 *
 *  The implementation keeps a memoised 2‑D table that is enlarged only as
 *  needed.  For any index n the corresponding (i,j) are computed, the
 *  table is enlarged to at least max(i,j) and the value A(i,j) is returned.
 */

#include <stdlib.h>

static seq_t *table = NULL;          /* 1‑based 2‑D array stored linearly    */
static size_t table_size = 0;        /* current maximum row/column index     */

/* enlarge the table so that indices 1..new_size are valid */
static void ensure_size(size_t new_size)
{
    if (new_size <= table_size) return;

    /* Allocate new contiguous block with (new_size+1)² elements
       (index 0 is unused) */
    size_t alloc = (new_size + 1) * (new_size + 1);
    seq_t *new_table = (seq_t *)malloc(alloc * sizeof(seq_t));
    if (!new_table) abort();

    /* Compute all entries in the new table using the recurrence */
    for (size_t i = 0; i <= new_size; ++i)
    {
        for (size_t j = 0; j <= new_size; ++j)
        {
            if (i == 0 || j == 0)
                new_table[i * (new_size + 1) + j] = 0;
            else if (i == 1)
                new_table[i * (new_size + 1) + j] = (seq_t)j;
            else if (j == 1)
                new_table[i * (new_size + 1) + j] = (seq_t)i;
            else
                new_table[i * (new_size + 1) + j] =
                    new_table[(i - 1) * (new_size + 1) + j] +
                    new_table[i * (new_size + 1) + (j - 1)] +
                    (seq_t)i + (seq_t)j;
        }
    }

    /* Release old storage and store the new one */
    free(table);
    table      = new_table;
    table_size = new_size;
}

/* Convert the linear index n to a pair (i,j) on the antidiagonal table */
static void index_to_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* Find the smallest s such that s(s-1)/2 >= n */
    idx_t s = 2;
    while (((seq_t)s * (s - 1)) / 2 < n)
        ++s;

    /* Previous diagonal cumulative count = (s-1)(s-2)/2 */
    idx_t prev = ((s - 1) * (s - 2)) / 2;
    idx_t offset = n - prev;           /* 1 .. (s-1) */
    *i = offset;
    *j = s - offset;
}

/* Main function required by the problem statement */
seq_t a(idx_t n)
{
    idx_t i, j;
    index_to_coords(n, &i, &j);

    /* Ensure table big enough for the requested coordinates */
    size_t need = (size_t)(i > j ? i : j);
    ensure_size(need);

    return table[i * (table_size + 1) + j];
}
