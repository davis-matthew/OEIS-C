/*
 * A336627  Coordination sequence for the Manhattan lattice.
 * https://oeis.org/A336627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 4*(n-1) for n >= 5, with initial values: a(0) = 1, a(1) =
 *            2, a(2) = 4, a(3) = 8, a(4) = 11
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)2;
        case 2: return (seq_t)4;
        case 3: return (seq_t)8;
        case 4: return (seq_t)11;
        default:
            return (seq_t)4 * ((seq_t)(n - 1));
    }
}
