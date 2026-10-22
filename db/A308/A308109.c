/*
 * A308109  Take all the integer-sided triangles with perimeter n and sides a,
 *          b, and c such that a < b < c. a(n) is the sum of all the b's.
 * https://oeis.org/A308109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all middle sides b of integer triangles with
 *            perimeter n, satisfying a < b < c and the triangle inequality.
 *            For each n >= 1, a(n) = Σ_{1≤a<b<c, a+b+c=n, a+b>c} b (zero for
 *            n < 9)
 * Range:     u8 n <= 34, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* No triangles exist for perimeters below 9.  */
    if (n < 9) {
        return 0;
    }

    seq_t total = 0;
    idx_t half = n / 2;             /* floor(n/2) */
    idx_t max_a = (n - 3) / 3;       /* floor((n-3)/3) */

    for (idx_t a = 1; a <= max_a; ++a) {
        /* Lower bound on b: must be > a and satisfy a+b>n/2.  */
        idx_t lb1 = a + 1;
        idx_t lb2 = half - a + 1;
        idx_t lb = (lb1 > lb2) ? lb1 : lb2;

        /* Upper bound on b: must be < c = n-a-b,
           equivalently 2b < n-a.  */
        idx_t ub = (n - a - 1) / 2;

        if (ub >= lb) {
            seq_t count = (seq_t)(ub - lb + 1);
            seq_t first_plus_last = (seq_t)(lb + ub);
            seq_t partial = (count * first_plus_last) / 2; /* sum of arithmetic progression */
            total += partial;
        }
    }

    return total;
}
