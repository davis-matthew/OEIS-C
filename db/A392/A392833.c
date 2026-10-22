/*
 * A392833  Cube of 3, then 33, then 333, increasing the number of threes by
 *          one each time.
 * https://oeis.org/A392833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((10^n-1)/3)^3 (the cube of the integer consisting of n
 *            3's)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 0;                     /* repunit of 3's with n digits */
    for (idx_t i = 0; i < n; ++i) {
        r = r * 10 + 3;              /* append a digit 3:  r = r*10 + 3   */
    }
    seq_t t = r * r;                 /* square first */
    return t * r;                    /* and finally multiply by r again */
}
