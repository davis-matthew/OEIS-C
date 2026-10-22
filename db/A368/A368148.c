/*
 * A368148  Square array A(n, k), n, k > 0, read and filled in the greedy way
 *          by upwards antidiagonals such that A(n, k) corresponds to the size
 *          of the connected component (relative to the Von Neumann
 *          neighborhood) of terms equal to A(n, k) including the position (n,
 *          k).
 * https://oeis.org/A368148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A368148 – Square array A(n,k) = size of the
    connected component of equal entries containing (n,k) in the
    greedy‑filled up‑wards anti‑diagonals.  The array is locally
    3×3 periodic:

          1  2  2
          2  1  3
          2  3  3

    Hence   A(n,k) = pat[(n‑1)%3][(k‑1)%3]   where pat is the table
    above.  The one‑dimensional sequence a(n) is obtained by
    enumerating the array in anti‑diagonal order and reading the
    values in that order.

    mapping n → (row,col) :
        let s be the smallest integer with (s‑1)*s/2 ≥ n
        prev = (s‑2)*(s‑1)/2
        offset = n – prev – 1   (0‑based within the diagonal)
        row = s‑1 – offset
        col = offset + 1
    Then a(n) = pat[(row‑1)%3][(col‑1)%3].
*/
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
/* Use the provided integer‑square‑root helper if desired:
   #include "../utils/int_sqrt.h"
   On most building systems int_sqrt is available from the
   utils directory.  We stay independent of it and use the C
   library sqrt for the small indices handled here. */

static const seq_t pat[3][3] = {
    {1, 2, 2},
    {2, 1, 3},
    {2, 3, 3}
};

seq_t a(idx_t n)
{
    /* Determine the anti‑diagonal number s so that (s‑1)*s/2 ≥ n */
    long double d = (long double)8 * n + 1.0L;
    long double sqrt_d = sqrtl(d);
    uint64_t s = (uint64_t)((sqrt_d + 1.0L) / 2.0L);
    uint64_t cum = (s - 1) * s / 2;
    if (cum < n)
        ++s;  /* adjust if rounding caused s to be too small */

    /* Compute the coordinates for this position */
    uint64_t prev = (s - 2) * (s - 1) / 2;
    uint64_t offset = n - prev - 1;          /* 0‑based offset on the diagonal */
    uint64_t row_coord = s - 1 - offset;     /* n‑coordinate (1‑based) */
    uint64_t col_coord = offset + 1;         /* k‑coordinate (1‑based) */

    /* Map into the 3×3 periodic pattern */
    uint64_t r = (row_coord - 1) % 3;
    uint64_t c = (col_coord - 1) % 3;
    return pat[r][c];
}
