/*
 * A294262  a(n) = 3*a(n-1) + 5*a(n-2) + a(n-3), with a(0) = a(1) = 1 and a(2)
 *          = 7, a linear recurrence which is a trisection of A005252.
 * https://oeis.org/A294262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 5*a(n-2) + a(n-3), a(0)=1, a(1)=1, a(2)=7
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0uL || n == 1uL)
        return (seq_t)1;
    if (n == 2uL)
        return (seq_t)7;

    /* Use a sliding window of the last three values of the recurrence.
       Works for any seq_t (plain integer or arbitrary‑precision class). */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)7;   /* a(n-1) */
    seq_t a3;

    for (idx_t i = 3uL; i <= n; ++i) {
        a3 = (seq_t)3 * a2 + (seq_t)5 * a1 + a0; /* a(i) */
        a0 = a1;  /* shift window */
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
