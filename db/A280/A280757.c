/*
 * A280757  Expansion of x*(2 + x)/(1 - 4*x^2 - x^3 + x^4).
 * https://oeis.org/A280757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in x*(2 + x)/(1 - 4*x^2 - x^3 + x^4)
 *            Recurrence (for n ≥ 4): a(n) = 4*a(n-2) + a(n-3) - a(n-4)
 *            Initial values: a(0)=0, a(1)=2, a(2)=1, a(3)=8
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 62, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)8;

    seq_t a0 = (seq_t)0;       /* a(n-4)   */
    seq_t a1 = (seq_t)2;       /* a(n-3)   */
    seq_t a2 = (seq_t)1;       /* a(n-2)   */
    seq_t a3 = (seq_t)8;       /* a(n-1)   */
    seq_t an = a3;             /* placeholder for a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)4 * a2 + a1 - a0; /* a(n) = 4*a(n-2) + a(n-3) - a(n-4) */

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
