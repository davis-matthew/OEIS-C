/*
 * A335840  Expansion of x*(1+2*x)/((1-2*x)*(1-x+4*x^2)).
 * https://oeis.org/A335840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-2) + 8*a(n-3) (n > 3) Initial values:
 *            a(1)=1, a(2)=5, a(3)=9
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Under the OEIS offset, a(1) is the first term.
       The behaviour for n <= 0 is unspecified; return 0. */
    if (n == 0)
        return (seq_t)0;

    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)5;
    if (n == 3)
        return (seq_t)9;

    /* Use the three previous terms to iteratively build up
       to the requested index.  No dynamic allocation is needed. */
    seq_t a_nm3 = (seq_t)1;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)5;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)9;   /* a(n-1) */
    seq_t a_n   = (seq_t)0;   /* current term */

    for (idx_t i = 4; i <= n; ++i) {
        a_n = (seq_t)(3 * a_nm1) - (seq_t)(6 * a_nm2) + (seq_t)(8 * a_nm3);
        /* shift the window */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
