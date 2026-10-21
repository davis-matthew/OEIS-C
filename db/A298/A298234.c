/*
 * A298234  Number of nX2 0..1 arrays with every element equal to 0, 2, 3, 4
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 5*a(n-2) + 4*a(n-3) * Initial values: a(1)=1,
 *            a(2)=5, a(3)=13 * (This implementation recomputes the recurrence
 *            on each call, * which is sufficient for n up to ~10000 and works
 *            for any * integer type seq_t, including arbitrary‑precision
 *            classes.)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for the sequence; return 0 */

    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)5;
    if (n == 3)
        return (seq_t)13;

    /* Sliding window variables for a(n‑3), a(n‑2), a(n‑1) */
    seq_t prev3 = (seq_t)1;          /* a(1) */
    seq_t prev2 = (seq_t)5;          /* a(2) */
    seq_t prev1 = (seq_t)13;         /* a(3) */
    seq_t cur;                       /* a(i) */

    for (idx_t i = 4; i <= n; ++i) {
        cur = prev1 + (seq_t)5 * prev2 + (seq_t)4 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
