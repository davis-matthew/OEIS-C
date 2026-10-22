/*
 * A309708  Sum of the odd parts appearing among the smallest parts of the
 *          partitions of n into 4 parts.
 * https://oeis.org/A309708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k odd ≤ floor(n/4)} k * floor(((n - 4k)^2 + 6*(n -
 *            4k) + 12) / 12)
 * Range:     u8 n <= 16, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; 4 * k <= n; k += 2) {        /* odd k only */
        idx_t m = n - 4 * k;                      /* remaining sum  */
        seq_t msq = (seq_t)m * (seq_t)m;          /* m^2          */
        seq_t num = msq + 6 * (seq_t)m + 12;      /* numerator    */
        seq_t pk   = num / 12;                    /* partitions with ≤3 parts */
        total += (seq_t)k * pk;                   /* accumulate */
    }
    return total;
}
