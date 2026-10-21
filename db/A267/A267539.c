/*
 * A267539  Decimal representation of the middle column of the "Rule 143"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) for n > 4 Initial values: a(0)=1,
 *            a(1)=3, a(2)=6, a(3)=12, a(4)=25
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)25;

    /* Recurrence applied from n = 5 onward. */
    seq_t prev2 = (seq_t)12; /* a(3) */
    seq_t prev1 = (seq_t)25; /* a(4) */
    seq_t curr;
    for (idx_t i = 5; i <= n; ++i) {
        curr = (seq_t)3 * prev1 - (seq_t)2 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
