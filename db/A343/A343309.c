/*
 * A343309  Number of partitions of n into 3 distinct parts x,y,z such that
 *          (x+y+z) | x*y*z.
 * https://oeis.org/A343309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of partitions of n into 3 distinct parts x,y,z (x<y<z),
 *            such that n | x*y*z
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 5)          /* smallest sum of 3 distinct positives is 1+2+3 = 6 */
        return 0;

    idx_t cnt = 0;       /* local counter, fits into idx_t for all expected n */

    for (idx_t i = 1; i <= n / 3; ++i) {
        idx_t maxj = (n - i) / 2;         /* j must satisfy j > i and k = n-i-j > j */
        for (idx_t j = i + 1; j <= maxj; ++j) {
            idx_t k = n - i - j;
            if (k <= j)                     /* ensures distinctness: k > j > i */
                continue;
            /* check divisibility: n divides i*j*k */
            if ((i * j * k) % n == 0)
                ++cnt;
        }
    }
    return (seq_t)cnt;
}
