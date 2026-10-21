/*
 * A289147  Number of (n+1) X (n+1) binary matrices M with at most one 1 in
 *          each of the first n rows and each of the first n columns and
 *          M[n+1,n+1] = 0.
 * https://oeis.org/A289147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+3)*a(n-1) - (n-1)^2*a(n-2), a(0)=1, a(1)=5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)5; /* a(1) */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t term1 = (seq_t)(2*k + 3) * a1;
        seq_t term2 = (seq_t)(k - 1) * (seq_t)(k - 1) * a0;
        seq_t ak = term1 - term2;
        a0 = a1;
        a1 = ak;
    }
    return a1;
}
