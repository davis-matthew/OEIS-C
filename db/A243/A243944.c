/*
 * A243944  a(n) = A084768(n)^2.
 * https://oeis.org/A243944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [P_n(7)]^2 where P_n is the Legendre polynomial. Using
 *            the recurrence P_0(x)=1, P_1(x)=x
 *            P_n(x)=((2n-1)*x*P_{n-1}(x)-(n-1)*P_{n-2}(x))/n with x=7. The
 *            result is then squared to obtain a(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only integer arithmetic, so it works for 8‑, 16‑, 32‑,
   64‑bit and arbitrary‑precision builds.  It runs in O(n) time.  */

seq_t a(idx_t n)
{
    if (n == 0u)               /* a(0) = 1 */
        return (seq_t)1;
    if (n == 1u)               /* a(1) = 7^2 = 49 */
        return (seq_t)7 * (seq_t)7;

    /* Legendre recurrence: P_0 = 1, P_1 = 7 */
    seq_t Pnm2 = (seq_t)1;          /* P_{i-2} */
    seq_t Pnm1 = (seq_t)7;          /* P_{i-1} */
    seq_t Pn   = 0;                 /* P_i  */

    for (idx_t i = 2u; i <= n; ++i)
    {
        /* term1 = (2i-1) * 7 * P_{i-1}   */
        seq_t term1 = Pnm1 * (seq_t)7 * (seq_t)(2*i - 1u);
        /* term2 = (i-1) * P_{i-2}         */
        seq_t term2 = Pnm2 * (seq_t)(i - 1u);

        /* P_i = (term1 - term2) / i      */
        Pn = (term1 - term2) / (seq_t)i;

        /* shift for next iteration */
        Pnm2 = Pnm1;
        Pnm1 = Pn;
    }

    /* a(n) = P_n(7)^2 */
    return Pn * Pn;
}
