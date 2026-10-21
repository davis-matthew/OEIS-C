/*
 * A265227  Nonnegative m for which k*floor(m^2/9) = floor(k*m^2/9), with 2 <
 *          k < 9.
 * https://oeis.org/A265227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)/5)*9 + {0,1,3,6,8}[(n-1)%5]
 * Range:     u8 n <= 143, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all nonnegative integers m with m mod 9 ∈ {0,1,3,6,8} */

seq_t a(idx_t n)
{
    static const unsigned int residues[5] = {0, 1, 3, 6, 8};
    idx_t i = n - 1;            // safe since n >= 1
    idx_t q = i / 5;            // number of full blocks of 5 kept residues
    idx_t r = i % 5;            // position within the current block
    return ((seq_t)q) * 9 + residues[r];
}
