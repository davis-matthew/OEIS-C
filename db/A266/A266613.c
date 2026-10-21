/*
 * A266613  Decimal representation of the middle column of the "Rule 41"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1)+a(n-2)-2*a(n-3), a(0)=1, a(1)=2, a(2)=5, a(3)=10, a(4)=20, a(5)=41 */
seq_t a(idx_t n)
{
    /* Handle first six terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)10;
    if (n == 4) return (seq_t)20;
    if (n == 5) return (seq_t)41;

    /* Compute iteratively for n >= 6 */
    seq_t prev3 = (seq_t)10;  /* a(3)  */
    seq_t prev2 = (seq_t)20;  /* a(4)  */
    seq_t prev1 = (seq_t)41;  /* a(5)  */

    seq_t current = 0;
    for (idx_t i = 6; i <= n; ++i)
    {
        current = (prev1 << 1) + prev2 - (prev3 << 1);
        prev3 = prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return current;
}
