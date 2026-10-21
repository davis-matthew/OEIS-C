/*
 * A267259  Number of ON (black) cells in the n-th iteration of the "Rule 111"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 for even n ≥ 6; a(n) = 2*n-3 for odd n ≥ 5; otherwise
 *            a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=5
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n)
    {
        case 0: return 1;
        case 1: return 2;
        case 2: return 3;
        case 3: return 5;
        case 4: return 5;
        default:
            if ((n & 1) == 0)
                return 6;
            else
                return ((seq_t)2 * (seq_t)n) - 3;
    }
}
