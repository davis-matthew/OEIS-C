/*
 * A273336  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 657", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(1) = 5; for n > 1, a(n) = 2/3 * (2*n^3 + 6*n^2 + 4*n
 *            - 15) (integer division) This formula matches all known terms
 *            and produces exact integers
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    seq_t nn = (seq_t)n;

    /* Compute the cubic polynomial part: 2*n^3 + 6*n^2 + 4*n - 15 */
    seq_t tp = 2 * nn * nn * nn;          /* 2*n^3 */
    tp += 6 * nn * nn;                    /* +6*n^2 */
    tp += 4 * nn;                         /* +4*n */
    tp -= (seq_t)15;                      /* -15 */

    /* Multiply by 2 and divide by 3 (division is exact) */
    seq_t result = (2 * tp) / (seq_t)3;
    return result;
}
