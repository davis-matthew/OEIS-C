/*
 * A399795  a(n) is the sum of indices k such that n belongs to the set R(k),
 *          where R(k) is the set of k consecutive integers centered at k,
 *          with k omitted when k is even.
 * https://oeis.org/A399795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - ceil((n-1)/3)^2 +
 *            (n-ceil((n+1)/2)+1)*(n+ceil((n+1)/2)) +
 *            max(0,(floor((n-1)/2)-ceil(n/3)+1)*(floor((n-1)/2)+ceil(n/3)))
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n^2 */
    seq_t n_sq = (seq_t)n * n;

    /* ceil((n-1)/3) = (n+1)/3 */
    idx_t c1 = (n + 1) / 3;
    seq_t term1 = n_sq - (seq_t)c1 * c1;

    /* ceil((n+1)/2) = (n+2)/2 */
    idx_t c2 = (n + 2) / 2;
    seq_t term2 = (seq_t)(n - c2 + 1) * (seq_t)(n + c2);

    /* floor((n-1)/2) and ceil(n/3) */
    long long d1 = (long long)(n - 1) / 2;          /* floor((n-1)/2) */
    long long d2 = (long long)(n + 2) / 3;          /* ceil(n/3)    */
    long long diff = d1 - d2 + 1;                   /* floor - ceil + 1 */

    seq_t term3 = 0;
    if (diff > 0)
    {
        term3 = (seq_t)diff * ( (seq_t)d1 + (seq_t)d2 );
    }

    return term1 + term2 + term3;
}
