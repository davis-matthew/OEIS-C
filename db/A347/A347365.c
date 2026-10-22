/*
 * A347365  a(n) = n * (2-(-1)^n), or zero together with first differences of
 *          even triangular numbers halved (A074378).
 * https://oeis.org/A347365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2-(-1)^n)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the output type once */
    seq_t nn = (seq_t)n;
    /* (-1)^n is 1 for even n and -1 for odd n.
       So 2-(-1)^n is 1 when n is even, 3 when n is odd. */
    if ((n & 1) == 0)           /* n is even */
        return nn;
    else                        /* n is odd */
        return nn * 3;
}
