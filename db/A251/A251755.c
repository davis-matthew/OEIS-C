/*
 * A251755  Digital root of n + n^2.
 * https://oeis.org/A251755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A010888(A002378(n)) (digital root of n + n^2)
 * Range:     u8 n <= 15, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t x  = nn * (nn + 1);          /* x = n + n² = n(n+1) */
    if (x == 0)
        return 0;                       /* digital root of 0 is 0 */
    seq_t r = x % 9;
    return r == 0 ? 9 : r;              /* digital root mapping 0→9 for positive numbers */
}
