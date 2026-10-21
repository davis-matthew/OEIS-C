/*
 * A266846  Decimal representation of the n-th iteration of the "Rule 70"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=6, a(2)=20, a(n)=2*a(n-1)+16*a(n-2)-32*a(n-3) for
 *            n>2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)20;
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)6;   /* a(1) */
    seq_t a2 = (seq_t)20;  /* a(2) */
    seq_t curr;
    idx_t i;
    for (i = 3; i <= n; ++i)
    {
        curr = a2 * (seq_t)2 + a1 * (seq_t)16 - a0 * (seq_t)32;
        a0 = a1;
        a1 = a2;
        a2 = curr;
    }
    return a2;
}
