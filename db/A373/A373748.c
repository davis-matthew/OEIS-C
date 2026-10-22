/*
 * A373748  Triangle read by rows: T(n, k) is k if k is a quadratic residue
 *          modulo n, otherwise is -k and is a quadratic nonresidue modulo n.
 *          T(0, 0) = 0 by convention.
 * https://oeis.org/A373748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: flatten the triangle T(n,k) on antidiagonals.
   For a given index n, determine row r and column k such that
   the flattened sequence a(n) is obtained, then return k if k is a
   quadratic residue modulo r, otherwise return -k.  */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* T(0,0) = 0 by definition */
        return 0;

    /* Find row r such that the cumulative length of rows 0..r-1
       is <= n < cumulative length of rows 0..r.  */
    idx_t r = 0;
    idx_t rem = n;
    while (rem >= r + 1) {
        rem -= r + 1;
        ++r;
    }
    /* Column k within the row r */
    idx_t k = rem;          /* 0 <= k <= r */

    /* Determine if k is a quadratic residue modulo r (i.e. there
       exists x with x*x ≡ k (mod r)).  For r=1 the only residue
       is 0; the loop below handles it as well.               */
    bool is_residue = false;
    idx_t val = k % r;
    for (idx_t x = 0; x < r; ++x) {
        if ((x * x) % r == val) {
            is_residue = true;
            break;
        }
    }

    /* Return k with appropriate sign */
    seq_t result = (seq_t)k;
    if (!is_residue)
        result = -(seq_t)k;
    return result;
}
