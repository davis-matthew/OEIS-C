/*
 * A398721  Number of increasing 3-term arithmetic progressions in an n^4
 *          hypercube.
 * https://oeis.org/A398721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((x^2 + y^2)^4 - n^4) / 2, where x = floor(n/2) and y =
 *            ceil(n/2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t NN = (seq_t)n;           /* convert index to seq_t */
    seq_t x  = NN / 2;             /* floor(n/2) */
    seq_t y  = NN - x;             /* ceil(n/2) */
    seq_t t  = x * x + y * y;      /* x^2 + y^2 */
    seq_t t2 = t * t;              /* (x^2 + y^2)^2 */
    seq_t t4 = t2 * t2;            /* (x^2 + y^2)^4 */
    seq_t n2 = NN * NN;            /* n^2 */
    seq_t n4 = n2 * n2;            /* n^4 */
    return (t4 - n4) / 2;          /* ((x^2 + y^2)^4 - n^4) / 2 */
}
