/*
 * A262000  a(n) = n^2*(7*n - 5)/2.
 * https://oeis.org/A262000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (7*n - 5) / 2
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 855, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;          /* cast index to the sequence type */
    seq_t sq = nt * nt;           /* n^2            */
    seq_t inner = 7 * nt - 5;     /* 7*n - 5        */
    seq_t num = sq * inner;       /* n^2 * (7*n-5)  */
    return num / 2;               /* exact division by 2 */
}
