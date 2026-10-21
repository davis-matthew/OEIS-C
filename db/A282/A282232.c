/*
 * A282232  a(n) = ((3*n + 1)^6 - 1)/9.
 * https://oeis.org/A282232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((3*n + 1)^6 - 1)/9
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 13, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute t = 3*n + 1 using seq_t to avoid overflow */
    seq_t t = (seq_t)3 * (seq_t)n + 1;
    /* t^2 */
    seq_t t2 = t * t;
    /* t^4 = (t^2)^2 */
    seq_t t4 = t2 * t2;
    /* t^6 = t^4 * t^2 */
    seq_t t6 = t4 * t2;
    /* Final formula */
    seq_t result = (t6 - 1) / 9;
    return result;
}
