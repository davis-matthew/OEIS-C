/*
 * A249457  The numerator of curvatures of touching circles inscribed in a
 *          special way in the larger segment of a unit circle divided by a
 *          chord of length sqrt(84)/5.
 * https://oeis.org/A249457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) follows the recurrence * a(0) = 10, a(1) = 100, a(2) = 2890
 *            * a(n) = 37*a(n-1) - 111*a(n-2) + 27*a(n-3) for n ≥ 3 * This is
 *            equivalent to the OGF given in the OEIS entry and * the
 *            definition a(n) = 5*(A249862(n)+3^n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)10;
    if (n == 1) return (seq_t)100;
    if (n == 2) return (seq_t)2890;
    seq_t a0 = (seq_t)10;   /* a(n-3)      */
    seq_t a1 = (seq_t)100;  /* a(n-2)      */
    seq_t a2 = (seq_t)2890; /* a(n-1)      */
    seq_t a3 = 0;
    idx_t i;
    for (i = 3; i <= n; ++i) {
        a3 = (seq_t)37 * a2   /* 37*a(n-1)  */
           - (seq_t)111 * a1  /* 111*a(n-2) */
           + (seq_t)27 * a0;  /* 27*a(n-3)  */
        a0 = a1;   /* shift window  */
        a1 = a2;
        a2 = a3;
    }
    return a2; /* a(n) */
}
