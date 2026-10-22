/*
 * A387614  The Pythagoras number of the maximal order of the field
 *          Q(sqrt(D)), where D = A005117(n) is the n-th squarefree number.
 * https://oeis.org/A387614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 for n >= 7; a(1)=4, a(2)=3, a(3)=3, a(4)=3, a(5)=4,
 *            a(6)=4
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n >= 7) return (seq_t)5;
    if (n <= 4) return (seq_t)3;   /* covers n = 2,3,4 */
    /* n is 5 or 6 */
    return (seq_t)4;
}
