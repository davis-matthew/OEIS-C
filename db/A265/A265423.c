/*
 * A265423  a(n) = (-1)^n + 50*floor(3*n/2) - 100*floor(n/4).
 * https://oeis.org/A265423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1)^n + 50*floor(3*n/2) - 100*floor(n/4)
 * Range:     u8 n <= 5, u16 n <= 1309, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t floor3n2 = sn + sn / 2;          /* floor(3*n/2) = n + n/2   */
    seq_t term = floor3n2 * (seq_t)50;     /* 50*floor(3*n/2)          */
    term -= (sn / 4) * (seq_t)100;         /* -100*floor(n/4)          */
    if ((n & 1) == 0)
        term += 1;                         /* +1 for even n            */
    else
        term -= 1;                         /* -1 for odd n             */
    return term;
}
