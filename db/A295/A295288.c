/*
 * A295288  Binomial transform of the centered triangular numbers A005448.
 * https://oeis.org/A295288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^2 + 9*n + 8) * 2^(n-3) (for n >= 0)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute t = 3*n^2 + 9*n + 8 as a seq_t to avoid intermediate overflow */
    seq_t t = ( (seq_t)3 * (seq_t)n * (seq_t)n )   /* 3*n^2 */
            + ( (seq_t)9 * (seq_t)n )              /* 9*n   */
            + (seq_t)8;                            /* +8    */

    /* If n < 3, shift right by (3-n) to divide by 2^(3-n);
     * else, shift left by (n-3) to multiply by 2^(n-3). */
    seq_t result;
    if (n < 3) {
        idx_t shift = 3 - n;   /* shift = 3,2,1 for n = 0,1,2 respectively */
        result = t >> shift;
    } else {
        idx_t shift = n - 3;
        result = t << shift;
    }
    return result;
}
