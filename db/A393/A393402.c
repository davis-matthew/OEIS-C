/*
 * A393402  Sum of the smallest parts in the partitions of n into 3 parts
 *          whose middle part is odd.
 * https://oeis.org/A393402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=1..⌊n/3⌋} Σ_{k=j..⌊(n-j)/2⌋} j * (k mod 2)
 * Range:     u8 n <= 36, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_j = n / 3;
    for (idx_t j = 1; j <= max_j; ++j)
    {
        idx_t max_k = (n - j) / 2;
        if (max_k < j)
            continue;                      /* no valid k */
        /* first odd k in [j, max_k] */
        idx_t start = j + ((j & 1) == 0);
        if (start > max_k)
            continue;                      /* no odd k */
        /* last odd k in [j, max_k] */
        idx_t end = max_k - ((max_k & 1) == 0);
        idx_t cnt = (end - start) / 2 + 1; /* number of odd k */
        sum += (seq_t)j * (seq_t)cnt;      /* add j for each such k */
    }
    return sum;
}
