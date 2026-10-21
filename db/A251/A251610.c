/*
 * A251610  Determinants of the spiral knots S(4,k,(1,1,1)).
 * https://oeis.org/A251610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is odd; a(n) = 2*n if n ≡ 2 (mod 4); a(n) = 0 if n
 *            ≡ 0 (mod 4)
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1)                /* odd */
        return (seq_t)n;
    if ((n & 3) == 0)         /* n divisible by 4 */
        return (seq_t)0;
    /* n ≡ 2 mod 4 */
    return ((seq_t)2) * (seq_t)n;
}
