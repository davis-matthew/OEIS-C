/*
 * A336288  Numbers of squares formed by this procedure on n-th step: Step 1,
 *          draw a unit square. Step n, draw a unit square with center in
 *          every intersection of lines of the figure in step n-1.
 * https://oeis.org/A336288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (8*n^3 - 12*n^2 + 7*n)/3, offset 1
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 979, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to seq_t early to avoid accidental
       64-bit overflow if seq_t is smaller */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;       /* n^2 */
    seq_t n3 = n2 * nn;       /* n^3 */

    seq_t num = (seq_t)8 * n3
              - (seq_t)12 * n2
              + (seq_t)7 * nn;

    return num / (seq_t)3;
}
