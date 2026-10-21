/*
 * A262864  Decimal representation of the middle column of the "Rule 147"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A262864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3) - 2*a(n-4); initial
 *            values: a(0)=1, a(1)=2, a(2)=4, a(3)=9
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small indices directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)9;

    /* Sliding window for the last four values. */
    seq_t a0 = (seq_t)1;  /* a(n-4) */
    seq_t a1 = (seq_t)2;  /* a(n-3) */
    seq_t a2 = (seq_t)4;  /* a(n-2) */
    seq_t a3 = (seq_t)9;  /* a(n-1) */
    seq_t ai;

    /* Compute up to the requested index. */
    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)3 * a3
            - (seq_t)3 * a2
            + (seq_t)3 * a1
            - (seq_t)2 * a0;

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3;
}
