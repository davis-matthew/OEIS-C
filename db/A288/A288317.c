/*
 * A288317  a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - a(n-4) for n >= 4, where
 *          a(0) = 2, a(1) = 4, a(2) = 7, a(3) = 12.
 * https://oeis.org/A288317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - a(n-4) for n >= 5, a(0) =
 *            2, a(1) = 4, a(2) = 7, a(3) = 12, a(4) = 22 (derived from the
 *            generating function)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 69, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)12;

    /* Base values for recurrence */
    seq_t a0 = (seq_t)2;   /* a(n-4)   */
    seq_t a1 = (seq_t)4;   /* a(n-3)   */
    seq_t a2 = (seq_t)7;   /* a(n-2)   */
    seq_t a3 = (seq_t)12;  /* a(n-1)   */

    /* Compute a(4) using the non‑homogeneous term from the generating
       function: a4 = a3 + 3*a2 - 2*a1 - a0 - 1 */
    seq_t a4 = a3 + (seq_t)3 * a2 - (seq_t)2 * a1 - a0 - (seq_t)1;
    if (n == 4) return a4;

    seq_t an;
    /* For n >= 5 use the homogeneous recurrence */
    for (idx_t i = 5; i <= n; ++i)
    {
        an = a4 + (seq_t)3 * a3 - (seq_t)2 * a2 - a1;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}
