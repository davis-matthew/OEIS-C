/*
 * A267538  Binary representation of the middle column of the "Rule 143"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=11, a(2)=110, a(3)=1100, a(4)=11001;
 *            a(n)=11*a(n-1)-10*a(n-2) for n>4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)110;
    if (n == 3) return (seq_t)1100;
    if (n == 4) return (seq_t)11001;

    seq_t prev2 = (seq_t)1100;   /* a(3) */
    seq_t prev1 = (seq_t)11001;  /* a(4) */
    seq_t curr;

    for (idx_t i = 5; i <= n; ++i) {
        curr = 11 * prev1 - 10 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
