/*
 * A242774  a(n) = ceiling( n / 2 ) + ceiling( n / 3 ).
 * https://oeis.org/A242774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n/2) + ceil(n/3)
 * Range:     u8 n <= 305, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ceil division for positive integers */
    seq_t half  = ((seq_t)n + 1) / 2;   /* ceil(n/2) */
    seq_t third = ((seq_t)n + 2) / 3;   /* ceil(n/3) */
    return half + third;
}
