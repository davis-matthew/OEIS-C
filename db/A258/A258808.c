/*
 * A258808  a(n) = n^7 - 1.
 * https://oeis.org/A258808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type once */
    seq_t sn = (seq_t)n;
    seq_t res = sn;          /* start with n^1 */

    /* multiply six more times to get n^7 */
    for (int i = 1; i < 7; ++i)
        res *= sn;

    /* subtract one to get a(n) = n^7 - 1 */
    res -= 1;

    return res;
}
