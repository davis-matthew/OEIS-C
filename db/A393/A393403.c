/*
 * A393403  Sum of the smallest parts in the partitions of n into 3 parts
 *          whose largest part is odd.
 * https://oeis.org/A393403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..⌊n/3⌋} j * count{ k | j≤k≤⌊(n-j)/2⌋ , (n-j-k)
 *            odd } The parity constraint reduces to a constant number of k's
 *            in the interval
 * Range:     u8 n <= 36, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;
    idx_t maxj = n / 3;                 /* upper bound for the smallest part */
    for (idx_t j = 1; j <= maxj; ++j) {
        idx_t kmax = (n - j) / 2;       /* upper bound for the middle part */
        if (kmax < j) continue;         /* no admissible middle part */

        idx_t range = kmax - j + 1;     /* count of k values in [j, kmax] */

        /* parity of k that makes c = n-j-k odd */
        idx_t p = (n - j - 1) & 1;      /* required parity of k */

        idx_t count;
        if ((j & 1) == p)
            count = (range + 1) / 2;    /* ceil(range/2) */
        else
            count = range / 2;          /* floor(range/2) */

        ans += (seq_t)j * (seq_t)count;
    }
    return ans;
}
