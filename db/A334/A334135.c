/*
 * A334135  Number of dimer tilings of a 2*n x 4 Moebius strip.
 * https://oeis.org/A334135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 34*a(n-2) + 14*a(n-3) - a(n-4) Initial
 *            values: a(0)=1, a(1)=7, a(2)=71, a(3)=769
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)71;
    if (n == 3) return (seq_t)769;

    /* Compute the recurrence iteratively. */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)7;
    seq_t a2 = (seq_t)71;
    seq_t a3 = (seq_t)769;
    seq_t a4;  /* will hold a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        a4 = (seq_t)14 * a3
           - (seq_t)34 * a2
           + (seq_t)14 * a1
           - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a4;
}
