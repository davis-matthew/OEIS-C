/*
 * A266753  Decimal representation of the n-th iteration of the "Rule 163"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)+2 for n>1, with a(0)=1, a(1)=4 (equivalently
 *            a(n) = (7*4^n - 4)/6 for n>=2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t val = (seq_t)4;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i) /* recurrence: a(i) = 4*a(i-1)+2 */
        val = val * (seq_t)4 + (seq_t)2;
    return val;
}
