/*
 * A334948  a(n) is the number of partitions of n into consecutive parts that
 *          differ by 6.
 * https://oeis.org/A334948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of partitions of n into consecutive parts that differ by 6
   Countable as the number of k ≥ 1 such that n − 3k(k−1) ≥ k and is divisible by k. */

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t k = 1; ; ++k) {
        unsigned __int128 tmp = (unsigned __int128)3 * k * (k - 1);
        if (tmp >= n) break;
        idx_t diff = n - (idx_t)tmp;          /* diff = n − 3k(k−1) */
        if (diff >= k && diff % k == 0) ++cnt;
    }
    return cnt;
}
