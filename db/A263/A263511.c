/*
 * A263511  Total number of ON (black) cells after n iterations of the "Rule
 *          155" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A263511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) for n>3; initial values:
 *            a(0)=1, a(1)=3, a(2)=6, a(3)=12
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* direct returns for the first four terms */
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)3;
    if (n == 2U) return (seq_t)6;
    if (n == 3U) return (seq_t)12;

    /* sliding window for the recurrence */
    seq_t a0 = (seq_t)1;   /* a(n-4)  */
    seq_t a1 = (seq_t)3;   /* a(n-3)  */
    seq_t a2 = (seq_t)6;   /* a(n-2)  */
    seq_t a3 = (seq_t)12;  /* a(n-1)  */
    seq_t ai = 0;

    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)2 * a3 - (seq_t)2 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return ai;
}
