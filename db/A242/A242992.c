/*
 * A242992  Least k>n/2, k<n, such that 2^(n-k)-1 divides 2^k-2, or 0 if no
 *          such k exists.
 * https://oeis.org/A242992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with n/2 < k < n such that (n-k)==1 or k % (n-k)
 *            == 1; a(n)=0 for n <= 2
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

    /* start searching at the smallest integer strictly larger than n/2 */
    idx_t start = n / 2 + 1;

    for (idx_t k = start; k < n; ++k)
    {
        idx_t t = n - k;   /* t = n - k > 0   */

        if (t == 1)
            return (seq_t)k;

        if (k % t == 1)
            return (seq_t)k;
    }

    /* no such k existed – return 0 */
    return 0;
}
