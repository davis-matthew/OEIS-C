/*
 * A264938  a(n) = n*(2*n-1) + floor(n/3).
 * https://oeis.org/A264938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n-1) + floor(n/3)
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n;          /* n as seq_t                */
    seq_t part = sq * 2;          /* 2*n                       */
    --part;                       /* 2*n - 1                   */
    seq_t prod = sq * part;       /* n*(2*n-1)                 */
    seq_t add  = (seq_t)(n / 3);  /* floor(n/3)                */
    return prod + add;            /* final result              */
}
