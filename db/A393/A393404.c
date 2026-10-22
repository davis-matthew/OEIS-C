/*
 * A393404  Sum of the smallest parts in the partitions of n into 3 parts
 *          whose largest part is even.
 * https://oeis.org/A393404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..floor(n/3)} Sum_{k=j..floor((n-j)/2)} j *
 *            ((n-j-k+1) mod 2)
 * Range:     u8 n <= 35, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0, a(1) = 0, ... as given in the OEIS entry */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_j = n / 3;                     /* smallest part goes up to floor(n/3) */
    for (idx_t j = 1; j <= max_j; ++j) {
        idx_t k_max = (n - j) / 2;            /* middle part up to floor((n-j)/2) */
        for (idx_t k = j; k <= k_max; ++k) {
            idx_t l = n - j - k;             /* largest part */
            if ((l & 1) == 0)                /* l even → add smallest part j */
                sum += (seq_t)j;
        }
    }
    return sum;
}
