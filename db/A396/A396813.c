/*
 * A396813  Lower (1/2,1) midsequence of (floor[n/2]^2) and (ceiling[n/2]^2);
 *          see Comments.
 * https://oeis.org/A396813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( ceil(n/2)^2 + floor(n/2)^2/2 )
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute integer division k = floor(n/2) and its complement */
    idx_t k  = n / 2;          /* floor(n/2) */
    idx_t k2 = n - k;          /* ceil(n/2) */

    /* perform multiplication in seq_t to avoid overflow of idx_t */
    seq_t term = ((seq_t)k * (seq_t)k) / 2 + ((seq_t)k2 * (seq_t)k2);
    return term;
}
