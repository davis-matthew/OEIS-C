/*
 * A283033  Number of inequivalent 5 X 5 matrices with entries in
 *          {1,2,3,...,n} up to rotations and reflections.
 * https://oeis.org/A283033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7*(n^18 + 4*n^8 + n^6 + 2)/8
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Computes the number of inequivalent 5x5 matrices with entries in 1..n
       up to rotations and reflections.  Uses the explicit polynomial from
       OEIS entry A283033. */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2   */
    seq_t n3 = n2 * nn;          /* n^3   */
    seq_t n4 = n2 * n2;          /* n^4   */
    seq_t n6 = n4 * n2;          /* n^6   */
    seq_t n7 = n6 * nn;          /* n^7   */
    seq_t n8 = n4 * n4;          /* n^8   */
    seq_t n9 = n6 * n3;          /* n^9   */
    seq_t n12 = n6 * n6;         /* n^12  */
    seq_t n18 = n12 * n6;        /* n^18  */

    seq_t x = n18 + (n8 << 2) + n6 + 2;   /* n^18 + 4*n^8 + n^6 + 2 */
    seq_t y = n7;                       /* n^7          */

    /* If y is divisible by 8 we can shift first to avoid overflow */
    if ((y & 7) == 0) {
        return (y >> 3) * x;           /* (y/8) * x    */
    } else {
        return (y * x) >> 3;           /* (y*x)/8      */
    }
}
