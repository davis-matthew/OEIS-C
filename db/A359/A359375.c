/*
 * A359375  Numbers that are neither multiples of 4 nor of the form 6u+3.
 * https://oeis.org/A359375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*floor((n-1)/7) + c[(n-1)%7] with c = {1,2,5,6,7,10,11}
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 as required */
    idx_t rem  = (n - 1) % 7;
    idx_t blk  = (n - 1) / 7;
    static const seq_t coeff[7] = {
        (seq_t)1, (seq_t)2, (seq_t)5,
        (seq_t)6, (seq_t)7, (seq_t)10,
        (seq_t)11
    };
    /* compute 12 * blk + coeff[rem] */
    return (seq_t)((blk * (idx_t)12) + (idx_t)coeff[rem]);
}
