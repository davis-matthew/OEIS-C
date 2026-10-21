/*
 * A254963  a(n) = n*(11*n + 3)/2.
 * https://oeis.org/A254963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(11*n + 3)/2
 * Range:     u8 n <= 4, u16 n <= 108, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t once to avoid repeated casts */
    seq_t ns = (seq_t)n;
    /* Compute n*(11*n + 3) first, then divide by 2 */
    seq_t val = ns * ((ns * 11) + 3);
    return val / 2;
}
