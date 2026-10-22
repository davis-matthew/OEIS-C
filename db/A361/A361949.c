/*
 * A361949  Triangle read by rows. T(n, k) = binomial(3*n - 1, 3*k - 1).
 * https://oeis.org/A361949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(3*row‑1, 3*col‑1) where (row, col) is the
 *            position of n in a triangle read by rows (row starts at 1, col
 *            starts at 1). The triangle entry is T(row, col) = C(3*row‑1,
 *            3*col‑1). The function computes row and col by subtracting
 *            successive row lengths from n
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* OEIS offset 1:  n = 1 gives the first element.  For n < 1 we
       return 0 as a safe default. */
    if (n < 1)
        return 0;

    /* Find the row r such that the sum 1 + 2 + … + r >= n, i.e.
       r(r+1)/2 >= n > (r-1)r/2.  Then column k = n - (r-1)r/2. */
    idx_t r = 1;
    idx_t rem = n;           /* remaining index while descending row bounds */
    while (rem > r) {
        rem -= r;
        ++r;
    }
    idx_t k = rem;           /* column index in the row */

    /* Compute the binomial coefficient C(3*r-1, 3*k-1).  The utility
       function handles large arguments and returns the result in seq_t. */
    idx_t top    = 3 * r - 1;
    idx_t bottom = 3 * k - 1;
    return binomial(top, bottom);
}
