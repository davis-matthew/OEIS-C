/*
 * A395114  Irregular triangle, next row is the previous row with the
 *          difference between consecutive numbers inserted between, starting
 *          with row 1 1.
 * https://oeis.org/A395114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf, look
 * Formula:   T(n,k) is defined recursively: * T(1,1)=T(1,2)=1; * for n>1: *
 *            if k is odd: T(n,k) = T(n-1,(k+1)/2) * if k is even: T(n,k) =
 *            T(n-1,k/2+1) - T(n-1,k/2) * a(n) is obtained by reading the rows
 *            of the triangle in order, starting * with row 1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _t_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_t_value.h"

seq_t a(idx_t n)
{
    if (n < 1)                      /* out of range, not used by caller */
        return (seq_t)0;

    /* Find the row r that contains the n-th element. */
    idx_t r = 1;
    idx_t cumulative = 0;           /* number of elements before the current row */

    while (1) {
        idx_t len;
        if (r - 1 >= 63)            /* avoid 64‑bit shift overflow for very large r */
            len = (idx_t)-1;
        else
            len = ((idx_t)1 << (r - 1)) + 1;

        if (cumulative + len >= n)
            break;
        cumulative += len;
        ++r;
    }

    idx_t k = n - cumulative;       /* 1‑based index within row r */
    return _t_value(r, k);
}
