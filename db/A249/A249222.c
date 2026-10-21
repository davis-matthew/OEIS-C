/*
 * A249222  Expansion of x*(1+5*x-5*x^3)/(1-6*x^2+5*x^4).
 * https://oeis.org/A249222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) - 5*a(n-4); a(1) = 1, a(2) = 5, a(3) = 6, a(4) =
 *            25 (indices are 1‑based)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)25;

    /* Use a sliding window of the last four values. */
    seq_t a1 = (seq_t)1;   /* a(i-4) */
    seq_t a2 = (seq_t)5;   /* a(i-3) */
    seq_t a3 = (seq_t)6;   /* a(i-2) */
    seq_t a4 = (seq_t)25;  /* a(i-1) */

    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = 6*a(i-2) - 5*a(i-4) */
        seq_t next = (seq_t)6 * a3 - (seq_t)5 * a1;

        /* Shift the window: drop the oldest value. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
