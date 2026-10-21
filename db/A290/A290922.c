/*
 * A290922  p-INVERT of the positive integers, where p(S) = 1 - S - 2*S^2.
 * https://oeis.org/A290922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + 5*a(n-3) - a(n-4) a(0) = 1, a(1) =
 *            5, a(2) = 20, a(3) = 75
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)20;
    if (n == 3) return (seq_t)75;

    /* Keep the last four computed values. */
    seq_t a0 = (seq_t)1;   /* a(n-4)  */
    seq_t a1 = (seq_t)5;   /* a(n-3)  */
    seq_t a2 = (seq_t)20;  /* a(n-2)  */
    seq_t a3 = (seq_t)75;  /* a(n-1)  */
    seq_t ai;              /* a(n)    */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* Compute a(i) = 5*a(i-1) - 6*a(i-2) + 5*a(i-3) - a(i-4) */
        /* Reorder to avoid negative intermediate values on unsigned seq_t. */
        seq_t left  = (seq_t)5 * a3 + (seq_t)5 * a1;
        seq_t right = (seq_t)6 * a2 + a0;
        ai = left - right;

        /* Shift window for next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3;
}
