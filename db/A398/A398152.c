/*
 * A398152  a(n) is the least positive integer k for which there exists a
 *          composition of k of depth n, where at each iteration the
 *          consecutive absolute differences must be nonzero and pairwise
 *          distinct and may then be permuted arbitrarily.
 * https://oeis.org/A398152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 1 + ceiling(n*(n + 1)/4) + floor(n/2)
 * Range:     u8 n <= 15, u16 n <= 411, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;              /* use seq_t from build for all arithmetic */
    seq_t prod = m * (m + (seq_t)1); /* n*(n+1) in seq_t */
    seq_t ceil_part = (prod + (seq_t)3) / (seq_t)4; /* ceil division by 4 */
    seq_t floor_part = m / (seq_t)2; /* floor(n/2) */
    return m + (seq_t)1 + ceil_part + floor_part;
}
