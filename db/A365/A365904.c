/*
 * A365904  Triangle read by rows T(n,k) = n^2 - binomial(k+1,2), n>=1, k<n.
 * https://oeis.org/A365904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r^2 - (k+1)*k/2, where r = ceil((sqrt(8*n+1)-1)/2) (row
 *            index) k = n - r*(r-1)/2 - 1 (column index)
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined, but safe */

    /* Find smallest r with r*(r+1)/2 >= n */
    idx_t r   = 1;
    idx_t cum = 1;                           /* r*(r+1)/2 */
    while (cum < n) {
        ++r;
        cum = r * (r + 1) / 2;
    }

    /* Number of terms before this row */
    idx_t prev = cum - r;                    /* (r-1)*r/2 */
    idx_t k    = n - prev - 1;               /* column index inside row */

    seq_t r_sq = (seq_t)r * (seq_t)r;
    seq_t k_seq = (seq_t)k;
    seq_t tri = (k_seq + 1) * k_seq / 2;     /* (k+1)*k/2 */

    return r_sq - tri;
}
