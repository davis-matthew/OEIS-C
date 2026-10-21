/*
 * A266592  Binary representation of the middle column of the "Rule 37"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1)+a(n-2)-10*a(n-3) for n > 3 Initial values:
 *            a(0)=1, a(1)=11, a(2)=111, a(3)=1110
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the known first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)111;
    if (n == 3) return (seq_t)1110;

    /* Sliding‑window variables for the recurrence */
    seq_t a0 = (seq_t)11;     /* a(n-3)  */
    seq_t a1 = (seq_t)111;    /* a(n-2)  */
    seq_t a2 = (seq_t)1110;   /* a(n-1)  */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 10*a(i-1) + a(i-2) - 10*a(i-3) */
        next = (seq_t)10 * a2 + a1 - (seq_t)10 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
