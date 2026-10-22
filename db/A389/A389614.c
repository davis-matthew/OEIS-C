/*
 * A389614  a(n) = 2*n^2 + 1 - (m*n - 3*m*(m+1)/2) where m = floor(n/3).
 * https://oeis.org/A389614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*n + 1 - (floor(n/3)*n -
 *            3*floor(n/3)*(floor(n/3)+1)/2)
 * Range:     u8 n <= 11, u16 n <= 188, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0 */

seq_t a(idx_t n)
{
    idx_t m_idx = n / 3;            /* floor(n/3) as idx_t */
    seq_t m  = (seq_t)m_idx;        /* cast to seq_t for arithmetic */
    seq_t n_s = (seq_t)n;

    /* 2*n^2 + 1 */
    seq_t term = 2 * n_s * n_s + 1;

    /* - floor(n/3)*n */
    seq_t part1 = m * n_s;

    /* + 3*floor(n/3)*(floor(n/3)+1)/2 */
    seq_t part2 = (seq_t)3 * m * (m + 1) / 2;

    /* combine */
    seq_t result = term - part1 + part2;

    return result;
}
