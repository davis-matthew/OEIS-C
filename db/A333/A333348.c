/*
 * A333348  Matching number of the tree of n vertices with the largest number
 *          of maximum matchings.
 * https://oeis.org/A333348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n+2)/7), except a(2)=1, a(6)=1, a(13)=3, a(20)=5
 * Range:     u8 n <= 730, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = ((seq_t)n * (seq_t)2 + (seq_t)2) / (seq_t)7;
    switch (n) {
    case 2u: return val + 1u;
    case 6u: return val - 1u;
    case 13u: return val - 1u;
    case 20u: return val - 1u;
    default: return val;
    }
}
