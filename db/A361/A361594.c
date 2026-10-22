/*
 * A361594  Expansion of e.g.f. exp( (x / (1-x))^2 ) / (1-x).
 * https://oeis.org/A361594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n-2)*a(n-1) - 3*(n-2)*(n-1)*a(n-2) * +
 *            (n-2)^2*(n-1)*a(n-3) (n ≥ 3) * Initial values: a(0)=1, a(1)=1,
 *            a(2)=4 * * This implementation uses an O(n) iterative recurrence
 *            with no * caching, so it works for any seq_t type that supports
 *            the * standard integer operations
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* Sliding window of past three values: a(n-3), a(n-2), a(n-1) */
    seq_t a_nm3 = (seq_t)1;  /* a(0) */
    seq_t a_nm2 = (seq_t)1;  /* a(1) */
    seq_t a_nm1 = (seq_t)4;  /* a(2) */
    seq_t a_n   = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i) {
        /* all intermediate factors are cast to seq_t to avoid
           mixing with idx_t when seq_t may be a class type */
        idx_t im1 = i - 1;
        idx_t im2 = i - 2;
        idx_t im3 = i - 3;

        seq_t term1 = (seq_t)(3 * i - 2) * a_nm1;
        seq_t term2 = (seq_t)3 * (seq_t)im2 * (seq_t)im1 * a_nm2;
        seq_t term3 = (seq_t)im2 * (seq_t)im2 * (seq_t)im1 * a_nm3;

        a_n = term1 - term2 + term3;

        /* shift the window for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
