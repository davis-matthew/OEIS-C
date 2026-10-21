/*
 * A250354  Number of length 5 arrays x(i), i=1..5 with x(i) in i..i+n and no
 *          value appearing more than 2 times.
 * https://oeis.org/A250354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + 5*n^4 + 25*n^2 + 5*n for n > 2 * (a(1) = 32, a(2) =
 *            216)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 32;
    if (n == 2) return 216;
    /* n might be zero or larger; for n >= 3 use the polynomial */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;        /* n^2 */
    seq_t n3 = n2 * sn;        /* n^3 */
    seq_t n4 = n3 * sn;        /* n^4 */
    seq_t n5 = n4 * sn;        /* n^5 */
    seq_t result = n5 + 5 * n4 + 25 * n2 + 5 * sn;
    return result;
}
