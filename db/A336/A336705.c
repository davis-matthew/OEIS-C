/*
 * A336705  Coordination sequence for the half-Manhattan lattice.
 * https://oeis.org/A336705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n for n >= 3; a(0)=1, a(1)=3, a(2)=7
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return 1;
        case 1: return 3;
        case 2: return 7;
        default: return (seq_t)4 * (seq_t)n;
    }
}
