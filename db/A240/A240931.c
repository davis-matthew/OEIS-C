/*
 * A240931  a(n) = n^8 - n^7.
 * https://oeis.org/A240931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 - n^7 = n^7 * (n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 256, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)   /* a(0) = a(1) = 0 for the OEIS sequence */
        return 0;

    seq_t x = (seq_t)n;
    seq_t pow7 = x;            /* start with n^1 */

    for (int i = 1; i < 7; ++i) /* multiply 6 more times to get n^7 */
        pow7 *= x;

    return pow7 * (x - (seq_t)1);
}
