/*
 * A255227  Number of (n+2)X(7+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 0 and no antidiagonal sum 0 and no row sum 2 and no column sum
 *          2.
 * https://oeis.org/A255227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4), a(1) = 127, a(2) =
 *            133, a(3) = 142, a(4) = 154 (Empirical formula from the OEIS
 *            entry)
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 52, u64 n <= 110, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n < 1 – the sequence is defined only for n ≥ 1. */
    if (n < 1)
        return 0;

    /* Base cases up to n = 4 */
    if (n == 1) return (seq_t)127;
    if (n == 2) return (seq_t)133;
    if (n == 3) return (seq_t)142;
    if (n == 4) return (seq_t)154;

    /* Compute using a sliding window of the last four values. */
    seq_t a1 = 127;          /* a(n-4)    */
    seq_t a2 = 133;          /* a(n-3)    */
    seq_t a3 = 142;          /* a(n-2)    */
    seq_t a4 = 154;          /* a(n-1)    */

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next = 2 * a4 - a3 + a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
