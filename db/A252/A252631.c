/*
 * A252631  Numbers n such that the heptagonal number H(n) is equal to the sum
 *          of the hexagonal numbers X(m), X(m+1), X(m+2) and X(m+3) for some
 *          m.
 * https://oeis.org/A252631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 323*a(n-1) - 323*a(n-2) + a(n-3), a(1)=92, a(2)=29524,
 *            a(3)=9506540
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n<1, return 0 */
    if (n == 1) return (seq_t)92;
    if (n == 2) return (seq_t)29524;
    if (n == 3) return (seq_t)9506540;

    /* start with the three known values */
    seq_t a1 = (seq_t)92;
    seq_t a2 = (seq_t)29524;
    seq_t a3 = (seq_t)9506540;

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t a4 = (seq_t)323 * a3 - (seq_t)323 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}
