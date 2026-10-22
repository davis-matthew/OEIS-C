/*
 * A397107  Lower (1,2) midsequence of (floor[n/2]^2) and (ceiling[n/2]^2);
 *          see Comments.
 * https://oeis.org/A397107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*k*k if n is even, otherwise a(n) = 3*k*k + 4*k + 2,
 *            where k = n / 2 (integer division). This is the lower (1,2)
 *            midsequence of (floor[n/2]^2) and (ceil[n/2]^2)
 * Range:     u8 n <= 18, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* See OEIS A397107 for details. */

seq_t a(idx_t n)
{
    seq_t k = ((seq_t)n) / 2;
    if ((n & 1) == 0) {
        return 3 * k * k;
    } else {
        return 3 * k * k + 4 * k + 2;
    }
}
