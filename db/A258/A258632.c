/*
 * A258632  Number of length 1 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5 or 7.
 * https://oeis.org/A258632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k ∈ [1, n+1] : k divisible by 2 or 3 or 5 or 7 }|
 * Range:     u8 n <= 210, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n + 1;
    seq_t cnt = 0;
    for (idx_t k = 1; k <= m; ++k)
    {
        if ((k % 2 == 0) || (k % 3 == 0) || (k % 5 == 0) || (k % 7 == 0))
            ++cnt;
    }
    return cnt;
}
