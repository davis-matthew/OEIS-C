/*
 * A395378  Numbers that are sums of two positive cubes in two or more ways
 *          corresponding to the Pell-type equation X^2 - 3*Y^2 = 56862.
 * https://oeis.org/A395378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = x(k)^3 + y(k)^3 where k = n-1 and X_k + Y_k√3 = (7 +
 *            4√3)^k. The pair (X_k, Y_k) satisfies the recurrence X_{k+1} =
 *            7*X_k + 12*Y_k Y_{k+1} = 4*X_k + 7*Y_k Using these, the
 *            corresponding cubes are x(k) = (810*X_k + 1134*Y_k - 162)/4 + 81
 *            y(k) = (378*X_k + 810*Y_k - 486)/4 (For k = 0 the val...
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 4, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is a special case because y(0) = -27.  */
    if (n == 1)
        return (seq_t)14329224;

    idx_t k = n - 1;            /* k ≥ 1 */

    /* Compute X_k and Y_k via the Pell unit (7 + 4√3).  */
    seq_t X = 1;                /* X_0 = 1, Y_0 = 0 */
    seq_t Y = 0;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t newX = 7 * X + 12 * Y;
        seq_t newY = 4 * X + 7 * Y;
        X = newX;
        Y = newY;
    }

    /* Compute x(k) and y(k).  They are positive for k ≥ 1.  */
    seq_t temp;

    /* x(k) = (810*X + 1134*Y - 162)/4 + 81 */
    temp = 810 * X;
    temp = temp + 1134 * Y;
    temp = temp - (seq_t)162;          /* safe: temp ≥ 162 for k ≥ 1 */
    seq_t x = temp / (seq_t)4 + (seq_t)81;

    /* y(k) = (378*X + 810*Y - 486)/4 */
    temp = 378 * X;
    temp = temp + 810 * Y;
    temp = temp - (seq_t)486;          /* safe: temp > 0 for k ≥ 1 */
    seq_t y = temp / (seq_t)4;

    /* Return the sum of cubes.  */
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;                /* x^3 */
    seq_t y2 = y * y;
    seq_t y3 = y2 * y;                /* y^3 */

    return x3 + y3;
}
