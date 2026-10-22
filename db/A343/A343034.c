/*
 * A343034  Positive numbers m such that m^2 with last digit z deleted is
 *          still a perfect square k^2, and z divides m-k.
 * https://oeis.org/A343034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n+4) = 38*a(n+2) - a(n). Initial values: a(1)=1, a(2)=13,
 *            a(3)=19, a(4)=487
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)19;
    if (n == 4) return (seq_t)487;

    /* a0, a1, a2, a3 represent a(n-4), a(n-3), a(n-2), a(n-1). */
    seq_t a0 = 1;      /* a(1)  */
    seq_t a1 = 13;     /* a(2)  */
    seq_t a2 = 19;     /* a(3)  */
    seq_t a3 = 487;    /* a(4)  */
    seq_t an;

    for (idx_t i = 5; i <= n; ++i) {
        an = 38 * a2 - a0;   /* a(n) = 38 * a(n-2) - a(n-4) */
        /* Shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;   /* a(n) */
}
