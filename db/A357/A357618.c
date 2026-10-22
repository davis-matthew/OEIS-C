/*
 * A357618  a(n) = sum of lengths of partitions of more than one consecutive
 *          positive integer adding up to n.
 * https://oeis.org/A357618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \sum_{k\ge 2\text{ and }n-k(k-1)/2>0}\;k if (n -
 *            k(k-1)/2) \bmod k == 0
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    seq_t sum = 0;
    seq_t S   = 1;   /* S = k*(k-1)/2 for k=2 */
    idx_t k   = 2;

    while (S < nn) {
        seq_t t   = nn - S;
        seq_t kk  = (seq_t)k;
        if (t % kk == 0) sum += kk;
        S += kk;          /* advance to next k: S = (k+1)*k/2 */
        k++;
    }
    return sum;
}
