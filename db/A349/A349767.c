/*
 * A349767  Numbers m such that 2^m - m is divisible by 5.
 * https://oeis.org/A349767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*floor((n-1)/4) + {3,14,16,17}[(n-1) mod 4]
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t residues[4] = { 3, 14, 16, 17 };
    /* Compute 20 * floor((n-1)/4) */
    seq_t q = ((seq_t)(n - 1) / 4) * 20;
    /* Add the proper residue */
    return q + residues[(n - 1) & 3];
}
