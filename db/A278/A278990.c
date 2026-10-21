/*
 * A278990  Number of loopless linear chord diagrams with n chords.
 * https://oeis.org/A278990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1)*a(n-1) + a(n-2) a(0) = 1, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t a_nm2 = (seq_t)1; /* a(0) */
    seq_t a_nm1 = (seq_t)0; /* a(1) */
    seq_t ai;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Compute ai = (2*i - 1) * a(i-1) + a(i-2) */
        seq_t factor = (seq_t)(2 * (idx_t)i - 1);
        ai = a_nm1 * factor + a_nm2;

        /* Shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = ai;
    }

    return a_nm1;
}
