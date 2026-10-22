/*
 * A367119  Place n points in general position on each side of an equilateral
 *          triangle, and join every pair of the 3*n+3 boundary points by a
 *          chord; sequence gives number of edges in the resulting planar
 *          graph.
 * https://oeis.org/A367119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3/2)*(3*n^4 + 4*n^3 + 3*n^2 + 4*n + 2)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use 64‑bit arithmetic to avoid overflow for n up to ~10^4 */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long n2 = nn * nn;          /* n^2 */
    unsigned long long n3 = n2 * nn;          /* n^3 */
    unsigned long long n4 = n3 * nn;          /* n^4 */

    unsigned long long P  = 3ULL * n4       /* 3*n^4   */
                         + 4ULL * n3       /* 4*n^3   */
                         + 3ULL * n2       /* 3*n^2   */
                         + 4ULL * nn       /* 4*n     */
                         + 2ULL;          /* +2      */

    unsigned long long result = (3ULL * P) / 2ULL; /* (3/2)*P */

    return (seq_t)result;
}
