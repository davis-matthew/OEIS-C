/*
 * A266794  Number of OFF (white) cells in the n-th iteration of the "Rule 61"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n=0, 1 for n=1, 3 for n=2, 2 for n=3, 5 for n=4;
 *            for n>4, a(n) = n+(-1)^n*(n-4) (i.e. even n: 2*n-4, odd n: 4)
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    switch (n) {
        case 0: return 0;
        case 1: return 1;
        case 2: return 3;
        case 3: return 2;
        case 4: return 5;
        default:
            if ((n & 1) == 0) {
                return (seq_t)n * 2 - 4;
            } else {
                return 4;
            }
    }
}
