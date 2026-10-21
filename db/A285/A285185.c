/*
 * A285185  Expansion of (2*x+4*x^2) / (1-2*x-2*x^2+2*x^3).
 * https://oeis.org/A285185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=2, a(2)=8, a(n)=2*a(n-1)+2*a(n-2)-2*a(n-3) for n>2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 48, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms explicitly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;

    /* Use a sliding window of the last three values. */
    seq_t a0 = (seq_t)0;   /* a(n-3) */
    seq_t a1 = (seq_t)2;   /* a(n-2) */
    seq_t a2 = (seq_t)8;   /* a(n-1) */
    seq_t an;              /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)2 * a2 + (seq_t)2 * a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return a2;
}
