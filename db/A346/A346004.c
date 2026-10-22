/*
 * A346004  If n even then n otherwise ((n+1)/2)^2.
 * https://oeis.org/A346004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n even ((n+1)/2)^2 if n odd
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n once to avoid repeated conversions */
    seq_t sn = (seq_t)n;
    if ((n & 1) == 0)
        return sn;
    /* (n+1)/2 in seq_t */
    seq_t half = (sn + 1) / 2;
    return half * half;
}
