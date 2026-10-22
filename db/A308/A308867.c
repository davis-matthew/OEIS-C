/*
 * A308867  Sum of all the parts in the partitions of n into 6 parts.
 * https://oeis.org/A308867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A238340(n) * where A238340(n) is the number of
 *            partitions of n into exactly 6 parts
 * Range:     u8 n <= 13, u16 n <= 38, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* For n < 6 there are no partitions into 6 positive parts. */
    if (n < 6) return 0;

    /* Rows: 0..n, Columns: 0..6 (7 columns). */
    size_t rows   = (size_t)n + 1;
    size_t cols   = 7;
    size_t total  = rows * cols;
    seq_t *p = (seq_t *)malloc(total * sizeof(seq_t));
    if (!p) return 0;   /* allocation failure → safe fallback */

    /* Initialize all entries to 0. */
    for (size_t k = 0; k < total; ++k) p[k] = 0;

    /* Base case: one way to partition 0 into 0 parts. */
    p[0 * cols + 0] = 1;

    /* Dynamic programming: p[i][j] = number of partitions of i into exactly j parts. */
    for (idx_t i = 1; i <= n; ++i)
    {
        for (idx_t j = 1; j <= 6; ++j)
        {
            if (i < j)           /* impossible to partition i into j positive parts */
                continue;
            if (i == j)           /* all parts must be 1 */
            {
                p[(size_t)i * cols + j] = 1;
            }
            else
            {
                /* recurrence: p(i,j) = p(i-1,j-1) + p(i-j,j) */
                seq_t term1 = p[(size_t)(i - 1) * cols + (j - 1)];
                seq_t term2 = p[(size_t)(i - j) * cols + j];
                p[(size_t)i * cols + j] = term1 + term2;
            }
        }
    }

    seq_t count  = p[(size_t)n * cols + 6];   /* number of 6‑part partitions of n */
    seq_t result = (seq_t)n * count;          /* sum of all parts in those partitions */

    free(p);
    return result;
}
