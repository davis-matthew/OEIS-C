/*
 * A380038  Triangle read by rows: T(n, k) = T(n-k, k) - T(n-k+1, k-1), T(n,
 *          0) = A010815(n-1), and T(0, 0) = 1.
 * https://oeis.org/A380038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 9647, i16 n <= 9647, i32 n <= 9647, i64 n <= 9647,
 *            i128 n <= 9647, big any n
 * Helpers:   compute_a010815
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A380038: Lower triangle read by rows.
    a(n) = T(r, c) where r = floor((sqrt(1+8n)-1)/2),
    c = n - r(r+1)/2  and  T(n,k) satisfies
     T(n,0) = A010815(n-1) (A010815: coeffs of Π (1 - x^k)),
     T(0,0) = 1,
     T(n,k) = 0 for n <= k(k-1)/2,
     T(n,k) = T(n-k, k) - T(n-k+1, k-1)  (n>0, k>0). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compute_a010815.h"

seq_t a(idx_t n)
{
    /* Determine row r and column c in the lower triangular array. */
    idx_t r = 0;
    idx_t tri = 0;                /* triangular number r(r+1)/2 */
    while (tri + (r + 1) <= n)
    {
        ++r;
        tri += r;                  /* tri = r(r+1)/2 after increment */
    }
    idx_t c = n - tri;            /* column within row r */

    if (r == 0)                   /* n == 0 */
    {
        return (seq_t)1;
    }

    /* Need A010815 values up to r-1 (for n-1 when n = r). */
    idx_t max_a010815 = r - 1;
    seq_t *a010815 = (seq_t *)calloc((size_t)(max_a010815 + 1), sizeof(seq_t));
    compute_a010815(a010815, max_a010815);

    /* Allocate a 2‑D triangular table: T[i][j] stored in a flat array. */
    size_t dim = (size_t)(r + 1);
    seq_t *T = (seq_t *)calloc(dim * dim, sizeof(seq_t));

    /* Helper macro for index access. */
    #define IDX(i, j) ((i) * dim + (j))

    /* Fill the table. */
    T[IDX(0, 0)] = (seq_t)1;               /* T(0,0) = 1 */
    for (idx_t i = 1; i <= r; ++i)
    {
        for (idx_t j = 0; j <= i; ++j)
        {
            if (j == 0)
            {
                /* T(i,0) = A010815(i-1) for i > 0 */
                T[IDX(i, 0)] = a010815[i - 1];
            }
            else
            {
                /* Zero for n <= k(k-1)/2 */
                idx_t thresh = j * (j - 1) / 2;
                if (i <= thresh)
                {
                    T[IDX(i, j)] = 0;
                }
                else
                {
                    /* Recurrence: T(i,j) = T(i-j,j) - T(i-j+1,j-1) */
                    seq_t part1 = T[IDX(i - j, j)];
                    seq_t part2 = T[IDX(i - j + 1, j - 1)];
                    T[IDX(i, j)] = part1 - part2;
                }
            }
        }
    }

    seq_t result = T[IDX(r, c)];

    free(a010815);
    free(T);
    #undef IDX
    return result;
}
