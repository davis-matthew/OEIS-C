/*
 * A266844  Binary representation of the n-th iteration of the "Rule 70"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1)+10000*a(n-2)-100000*a(n-3); initial values:
 *            a(0)=1, a(1)=110, a(2)=10100
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)110;
    if (n == 2) return (seq_t)10100;

    seq_t prev3 = (seq_t)1;      /* a(n-3)   */
    seq_t prev2 = (seq_t)110;    /* a(n-2)   */
    seq_t prev1 = (seq_t)10100;  /* a(n-1)   */
    seq_t next   = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        next = prev1 * (seq_t)10
             + prev2 * (seq_t)10000
             - prev3 * (seq_t)100000;

        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return next;
}
