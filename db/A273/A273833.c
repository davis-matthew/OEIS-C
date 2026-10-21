/*
 * A273833  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 961", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16*n^3+48*n^2-10*n-3*((-1)^n-5))/12 for n>1 a(0) = 1,
 *            a(1) = 5
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;
    /* (-3)*(((-1)^n)-5) simplifies to 12 for even n, 18 for odd n */
    seq_t delta = (n & 1) ? (seq_t)18 : (seq_t)12;
    seq_t num = 16 * n3 + 48 * n2 - 10 * ns + delta;
    return num / 12;
}
