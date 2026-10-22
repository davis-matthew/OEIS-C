/*
 * A377885  Cogrowth sequence of the 16-element quasihedral group SD16 = <S,T
 *          | S^8, T^2, STS^5T>.
 * https://oeis.org/A377885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=1, a(3)=4; for n>=4: a(n) = 6*a(n-1) -
 *            12*a(n-2) + 16*a(n-3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)4;

    seq_t prev3 = (seq_t)1;   /* a(1)   */
    seq_t prev2 = (seq_t)1;   /* a(2)   */
    seq_t prev1 = (seq_t)4;   /* a(3)   */
    seq_t cur   = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)6 * prev1;
        cur -= (seq_t)12 * prev2;
        cur += (seq_t)16 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
