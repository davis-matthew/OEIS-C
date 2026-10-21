/*
 * A278712  Triangle T read by rows: T(n, m), for n >= 2, and m = 1, 2, ...,
 *          n-1, equals the square root of the positive integer solution y of
 *          y^2 = x^3 - A(n, m)^2*x with the area A(n, m) = A249869(n, m) of
 *          the primitive Pythagorean triangle characterized by (n, m) or 0 if
 *          no such triangle exists.
 * https://oeis.org/A278712
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is obtained by reading the triangular array
 *  T(n,m), n>=2, m=1..n-1, with
 *
 *      T(n,m) = (n^2 - m^2) * n  if n>m>=1, gcd(n,m)=1 and n+m is odd,
 *      T(n,m) = 0                otherwise.
 *
 *  The sequence a(n) is the (n‑2)-th element of this triangle,
 *  starting with a(2)=T(2,1). The function below maps the linear
 *  index n to (row,column) of the triangle and returns the
 *  corresponding value.  No hard‑coded table is used and the
 *  algorithm runs in O(√n) time for each call.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        /* For indices below the offset the OEIS entry does not
           specify a value; return 0 to avoid undefined behaviour. */
        return 0;
    }

    /* Zero‑based offset of n in the sequence (a(2) -> offset 0). */
    idx_t k = n - 2;

    /* Find the row r such that the k‑th element lies in that row.
       cumulative is the total number of terms up to the current row. */
    idx_t r = 2;
    idx_t cumulative = 1;          /* (r-1)*r/2 for r=2 */

    while (k >= cumulative) {
        ++r;
        cumulative += (r - 1);
    }

    /* Index of the first element in the current row. */
    idx_t before = cumulative - (r - 1);

    /* Column within the row: m ranges from 1 to r-1. */
    idx_t m = (k - before) + 1;

    /* Convert to seq_t for arithmetic. */
    seq_t rn = (seq_t)r;
    seq_t mn = (seq_t)m;

    /* Apply the defining conditions of T(n,m). */
    if (gcd(rn, mn) != (seq_t)1) {
        return 0;
    }
    if (((rn + mn) & (seq_t)1) == 0) {
        return 0;
    }

    /* Compute (n^2 - m^2) * n. */
    seq_t diff = rn * rn - mn * mn;
    seq_t result = diff * rn;
    return result;
}
