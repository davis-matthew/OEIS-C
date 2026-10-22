/*
 * A374381  Triangle T(n, k), n > 0, k = 0..n-1, read by rows; T(n, k) = (n
 *          mod m) - (k mod m) where m = A007978(n-k).
 * https://oeis.org/A374381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   next_not_dividing
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A374381:  a(n) is the nth element of the triangle T(n,k)
    defined by
         T(n,k) = (n mod m) – (k mod m)
    where m = A007978(n−k) is the smallest integer >1 that does
    not divide (n−k).  The sequence is read row‑by‑row.
*/

#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/next_not_dividing.h"

/*  Return the nth term of A374381 (n >= 1).  Input n is the
    position in the flattened triangle. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* undefined for n <= 0, return 0 */

    /* Find row number r such that r*(r+1)/2 >= n and (r-1)*r/2 < n. */
    long double w = sqrtl(8.0L * n + 1.0L);
    idx_t r = (idx_t)((w - 1.0L) / 2.0L);   /* floor approximation */

    /* Repair possible rounding errors:  r might be too small. */
    while ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;

    idx_t prev = (r - 1) * r / 2;   /* number of terms before row r */
    idx_t k = n - prev - 1;         /* column index in this row (0‑based) */
    idx_t d = r - k;                /* argument for A007978 */

    idx_t m = next_not_dividing(d);

    /* Compute (r mod m) - (k mod m).  Cast to seq_t for arithmetic. */
    seq_t rem_r = (seq_t)(r % m);
    seq_t rem_k = (seq_t)(k % m);
    return rem_r - rem_k;
}
