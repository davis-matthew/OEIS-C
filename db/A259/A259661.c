/*
 * A259661  Binary representation of the middle column of the "Rule 54"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A259661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 11*a(n-2) + 11*a(n-3) - 10*a(n-4) (n>3) with
 *            initial values a(0)=1, a(1)=11, a(2)=110, a(3)=1100
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;      /* 1 */
    if (n == 1) return (seq_t)11;     /* 11 */
    if (n == 2) return (seq_t)110;    /* 110 */
    if (n == 3) return (seq_t)1100;   /* 1100 */

    /* For n ≥ 4 use the linear recurrence iteratively. */
    seq_t a0 = (seq_t)1;     /* a(0)   */
    seq_t a1 = (seq_t)11;    /* a(1)   */
    seq_t a2 = (seq_t)110;   /* a(2)   */
    seq_t a3 = (seq_t)1100;  /* a(3)   */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(i) = 11*a(i-1) - 11*a(i-2) + 11*a(i-3) - 10*a(i-4) */
        next = 11 * a3 - 11 * a2 + 11 * a1 - 10 * a0;

        /* Shift the four last terms for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
