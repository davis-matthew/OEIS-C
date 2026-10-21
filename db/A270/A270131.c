/*
 * A270131  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 89", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=10, a(n) = (4*n^3+6*n^2-n-24)/6 for n>2
 *            even, a(n) = (4*n^3+18*n^2+23*n-15)/6 for n>2 odd
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)5;
    if (n == 2u) return (seq_t)10;

    seq_t sn  = (seq_t)n;
    seq_t sn2 = sn * sn;
    seq_t sn3 = sn2 * sn;

    if ((n & 1u) == 0u) {          /* even n > 2 */
        seq_t num = 4 * sn3 + 6 * sn2 - sn - 24;
        return num / 6;
    } else {                        /* odd n > 2 */
        seq_t num = 4 * sn3 + 18 * sn2 + 23 * sn - 15;
        return num / 6;
    }
}
