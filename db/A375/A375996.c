/*
 * A375996  Number of integers y > 0 such that y < (n/2+x)*sqrt(3) and y <
 *          (n/2-x)*sqrt(3).
 * https://oeis.org/A375996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of integer points (x,y) with y>0 inside the *
 *            equilateral triangle of side length n whose vertices are at *
 *            (-n/2,0), (n/2,0) and (0,n*sqrt(3)/2). * For each integer x with
 *            |x| < n/2 the number of admissible y is * floor( (n-2|x|) *
 *            sqrt(3) / 2 ). * The code implements this counting dire...
 * Range:     u8 n <= 4, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n >= 1  (offset 1) */
    if (n == 0) return 0; /* safety, though n should be >=1 */

    /* maximum absolute value of x in the triangle */
    idx_t m = (n - 1) / 2;                /* floor((n-1)/2) */

    seq_t total = 0;

    /* contributions from x = 0 */
    {
        seq_t N = (seq_t)n;                 /* N = n - 2*0 */
        seq_t s = floor_sqrt((seq_t)3 * N * N);
        total += s >> 1;                    /* divide by 2 */
    }

    /* contributions from x != 0, using symmetry */
    for (idx_t i = 1; i <= m; ++i) {
        seq_t absx = (seq_t)i;              /* |x| = i */
        seq_t N = (seq_t)n - (seq_t)2 * absx;  /* N = n - 2|x| */
        seq_t s = floor_sqrt((seq_t)3 * N * N);
        seq_t cnt = s >> 1;                 /* floor(N*sqrt3/2) */

        total += cnt * (seq_t)2;            /* count for +x and -x */
    }

    return total;
}
