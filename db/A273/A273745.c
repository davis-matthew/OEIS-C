/*
 * A273745  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 901", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 for n=0, 17 for n=1, 8*(n+1) for n>=2
 * Range:     u8 n <= 30, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)7;
    if (n == 1)
        return (seq_t)17;
    /* For n >= 2: a(n) = 8 * (n + 1) */
    seq_t nn = (seq_t)n;
    return (nn + (seq_t)1) * (seq_t)8;
}
