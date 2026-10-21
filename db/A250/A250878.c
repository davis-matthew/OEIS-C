/*
 * A250878  Number of (1+1) X (n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (34*n^3 + 84*n^2 + 104*n + 48)/3 (OEIS A250878)
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute using a wider type to avoid overflow on small seq_t */
    unsigned long long N = n;                   /* safe for n up to ~10^5 */
    unsigned long long num = 34ULL * N * N * N
                           + 84ULL * N * N
                           + 104ULL * N
                           + 48ULL;
    return (seq_t)(num / 3ULL);
}
