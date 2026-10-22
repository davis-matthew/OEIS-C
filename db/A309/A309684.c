/*
 * A309684  Sum of the odd parts appearing among the smallest parts of the
 *          partitions of n into 3 parts.
 * https://oeis.org/A309684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{odd j=1..floor(n/3)} j*( floor((n-j)/2) - j + 1 )
 * Range:     u8 n <= 36, u16 n <= 240, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    /* j runs from 1 to floor(n/3) */
    idx_t maxj = n / 3;
    for (idx_t j = 1; j <= maxj; ++j) {
        if (j & 1U) {                     /* j is odd */
            idx_t v = (n - j) / 2;       /* floor((n - j)/2) */
            if (v >= j) {                /* should always hold for j <= n/3 */
                seq_t term = (seq_t)j * ((seq_t)(v - j + 1));
                res += term;
            }
        }
    }
    return res;
}
