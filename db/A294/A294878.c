/*
 * A294878  Characteristic function for A001608, Perrin sequence.
 * https://oeis.org/A294878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a Perrin number (0, 2, 3, 5, 7, 10, 12, 17, 22,
 *            30, ...), 0 otherwise
 * Range:     u8 n <= 9185, u16 n <= 9185, u32 n <= 9185, u64 n <= 9185,
 *            u128 n <= 9185, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small Perrin numbers handled directly. */
    if (n == 0 || n == 2 || n == 3) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Generate Perrin numbers until we pass n. */
    idx_t p0 = 3;          /* P(n-2) */
    idx_t p1 = 0;          /* P(n-1) */
    idx_t p2 = 2;          /* P(n)   */
    while (p2 <= n)       /* generate next values as needed */
    {
        idx_t next = p0 + p1;          /* P(k) = P(k-2) + P(k-3) */
        if (next == n) return (seq_t)1;
        if (next > n)  break;

        /* shift the window for the next iteration */
        p0 = p1;
        p1 = p2;
        p2 = next;
    }
    return (seq_t)0;
}
