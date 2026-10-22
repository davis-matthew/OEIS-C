/*
 * A368046  a(n) = ((n + 1)^2 * (5*n + 4)*n) / 12.
 * https://oeis.org/A368046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n + 1)^2 * (5*n + 4)*n) / 12
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t before any arithmetic */
    seq_t nn = n;
    /* Compute (n+1)^2 */
    seq_t res = (nn + 1) * (nn + 1);
    /* Multiply by (5*n + 4) */
    res *= (seq_t)(5 * nn + 4);
    /* Multiply by n */
    res *= nn;
    /* Divide by 12 (the result is always an integer) */
    res /= 12;
    return res;
}
