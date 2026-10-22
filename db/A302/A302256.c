/*
 * A302256  Hyper-Wiener index of rows of unit cells on the face-centered
 *          cubic lattice.
 * https://oeis.org/A302256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (81*n^4+261*n^3+264*n^2+540*n+132)/6
 * Range:     u8 none, u16 n <= 4, u32 n <= 107, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    seq_t n2 = t * t;      /* n^2 */
    seq_t n3 = n2 * t;     /* n^3 */
    seq_t n4 = n3 * t;     /* n^4 */
    seq_t sum = 81 * n4;   /* 81*n^4 */
    sum += 261 * n3;       /* +261*n^3 */
    sum += 264 * n2;       /* +264*n^2 */
    sum += 540 * t;        /* +540*n */
    sum += 132;            /* +132 */
    return sum / 6;        /* divide by 6 */
}
