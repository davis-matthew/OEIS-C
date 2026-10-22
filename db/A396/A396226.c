/*
 * A396226  a(n) = (n!)^7.
 * https://oeis.org/A396226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n!)^7
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t res = fact;
    /* multiply 6 more times to obtain fact^7 */
    for (int i = 1; i < 7; ++i)
        res *= fact;

    return res;
}
