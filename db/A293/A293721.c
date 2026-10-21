/*
 * A293721  E.g.f.: exp(x + 4*x^2 + 9*x^3).
 * https://oeis.org/A293721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 8*(n-1)*a(n-2) + 27*(n-1)*(n-2)*a(n-3), with
 *            a(0)=1, a(1)=1, a(2)=9. This recurrence follows from the
 *            exponential generating function exp(x + 4*x^2 + 9*x^3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;

    /* a0 = a(k-3), a1 = a(k-2), a2 = a(k-1) */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)9;   /* a(2) */

    for (idx_t k = 3; k <= n; ++k) {
        seq_t term1 = a2;                                 /* a(k-1)      */
        seq_t term2 = a1 * ((seq_t)(k - 1));              /* a(k-2)*(k-1)*/
        term2 *= (seq_t)8;                                /* multiplied by 8*/
        seq_t term3 = a0 * ((seq_t)(k - 1));              /* a(k-3)*(k-1)*/
        term3 *= (seq_t)(k - 2);                          /* a(k-3)*(k-1)*(k-2)*/
        term3 *= (seq_t)27;                               /* multiplied by 27*/
        seq_t ak = term1 + term2 + term3;                 /* a(k) */

        /* Shift for next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = ak;
    }

    return a2;
}
