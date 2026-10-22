/*
 * A322018  a(n) = A006068(A129760(A003188(n))).
 * https://oeis.org/A322018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006068(A129760(A003188(n)))
 * Range:     u8 n <= 255, u16 n <= 9202, u32 n <= 9202, u64 n <= 9202,
 *            u128 n <= 9202, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n + 1;
    seq_t low = m & (~(m - 1));   /* lowest set bit of m */

    /* a(n) = 0 iff n+1 is a power of two */
    if (m == low)
        return 0;

    /* n even: set all bits below the lowest set bit */
    if ((n & 1) == 0)
        return (seq_t)n | ((seq_t)n - 1);

    /* n odd: subtract the lowbit of n+1 and add one */
    return (seq_t)n - low + 1;
}
