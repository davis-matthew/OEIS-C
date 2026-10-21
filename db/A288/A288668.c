/*
 * A288668  a(n) = a(n-2) + 2*a(n-3) for n >= 3, where a(0) = 2, a(2) = 4,
 *          a(3) = 5.
 * https://oeis.org/A288668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-3) for n >= 3, with a(0) = 2, a(1) = 4,
 *            a(2) = 5
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 50, u64 n <= 103,
 *            u128 n <= 209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)5;

    /* aMinus3 = a(i-3), aMinus2 = a(i-2), aMinus1 = a(i-1) */
    seq_t aMinus3 = (seq_t)2;   /* a(0) */
    seq_t aMinus2 = (seq_t)4;   /* a(1) */
    seq_t aMinus1 = (seq_t)5;   /* a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = a(i-2) + 2 * a(i-3) */
        curr = aMinus2 + (seq_t)2 * aMinus3;
        /* Rotate the sliding window for the next iteration */
        aMinus3 = aMinus2;
        aMinus2 = aMinus1;
        aMinus1 = curr;
    }
    return curr;
}
