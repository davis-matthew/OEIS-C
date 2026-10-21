/*
 * A284379  Numbers k with digits 3 and 5 only.
 * https://oeis.org/A284379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=3, a(2)=5, a(2k+1)=10*a(k)+3, a(2k+2)=10*a(k)+5
 * Range:     u8 n <= 6, u16 n <= 62, u32 n <= 1534, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;
    idx_t parent = (n - 1) / 2;
    seq_t pval = a(parent);
    if (n & 1)           /* odd: 2k+1 */
        return pval * ((seq_t)10) + (seq_t)3;
    else                 /* even: 2k+2 */
        return pval * ((seq_t)10) + (seq_t)5;
}
