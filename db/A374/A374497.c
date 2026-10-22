/*
 * A374497  Expansion of 1/(1 - 4*x - 4*x^2)^(3/2).
 * https://oeis.org/A374497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 6; a(n) = (2*(2*n+1)*a(n-1) + 4*(n+1)*a(n-2))/n
 *            (n ≥ 2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)6;

    /* Iterative computation using only the two most recent terms */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2) : a(0)          */
    seq_t a_nm1 = (seq_t)6;   /* a(n-1) : a(1)          */
    seq_t a_n   = 0;          /* current term           */

    for (idx_t k = 2U; k <= n; ++k)
    {
        seq_t sn = (seq_t)k;

        /* term1 = 2 * (2*sn + 1) * a(n-1) */
        seq_t term1 = ((seq_t)2 *
                       (((seq_t)2 * sn) + (seq_t)1))
                      * a_nm1;

        /* term2 = 4 * (sn + 1) * a(n-2) */
        seq_t term2 = ((seq_t)4 *
                       (sn + (seq_t)1))
                      * a_nm2;

        /* recurrence */
        a_n = (term1 + term2) / sn;

        /* shift previous values */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
