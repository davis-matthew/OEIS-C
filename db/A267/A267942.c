/*
 * A267942  Interleave (n-1)^2 + 2 and (n+1)^2 + 2.
 * https://oeis.org/A267942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = (k-1)^2 + 2 a(2k+1) = (k+1)^2 + 2 (Replicates OEIS
 *            A267942)
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Even indices: a(2k) = (k-1)^2 + 2 */
    if ((n & 1ULL) == 0) {
        idx_t k = n >> 1;                 /* k = n/2  */
        long long kk = (long long)k - 1;   /* signed to allow -1 when k==0 */
        long long sq = kk * kk;            /* (k-1)^2  */
        return (seq_t)(sq + 2);
    }
    /* Odd indices: a(2k+1) = (k+1)^2 + 2 */
    {
        idx_t k = (n + 1ULL) >> 1;         /* k = (n+1)/2  */
        long long kk = (long long)k;       /* signed copy */
        long long sq = kk * kk;            /* k^2       */
        return (seq_t)(sq + 2);
    }
}
