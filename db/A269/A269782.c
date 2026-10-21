/*
 * A269782  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 65", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=5, a(3)=36, a(4)=9; a(n) = 2*n+5 for even n
 *            ≥ 5, a(n) = 4*n^2-2*n+6 for odd n ≥ 5
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 5;
    if (n == 3) return 36;
    if (n == 4) return 9;

    seq_t N = (seq_t)n;
    if ((n & 1ULL) == 0)                /* even index */
        return 2 * N + 5;
    else                                /* odd index */
        return 4 * N * N - 2 * N + 6;
}
