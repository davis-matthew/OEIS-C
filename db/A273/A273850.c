/*
 * A273850  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 969", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(n+1) for n>=5, with initial terms a(0)=3, a(1)=17,
 *            a(2)=24, a(3)=31, a(4)=45
 * Range:     u8 n <= 30, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return 3;
        case 1: return 17;
        case 2: return 24;
        case 3: return 31;
        case 4: return 45;
        default: return (seq_t)8 * ((seq_t)n + 1);
    }
}
