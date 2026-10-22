/*
 * A304487  a(n) = (3 + 2*n - 3*n^2 + 4*n^3 - 3*((-1 + n) mod 2))/6.
 * https://oeis.org/A304487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 + 2*n - 3*n^2 + 4*n^3 - 3*((-1 + n) mod 2))/6
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1213, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;          /*  n  */
    seq_t n2  = sn * sn;           /*  n^2  */
    seq_t n3  = n2 * sn;           /*  n^3  */
    seq_t numer = (seq_t)4 * n3 - (seq_t)3 * n2 + (seq_t)2 * sn + (seq_t)3;
    if ((n & 1ULL) == 0)           /* n even: subtract 3 */
        numer -= (seq_t)3;
    return numer / (seq_t)6;
}
