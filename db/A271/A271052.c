/*
 * A271052  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 253", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A271052: partial sums of active cells in Rule 253 CA
   Formula (for n > 1):
     if n is even:  a(n) = (8*n³ + 12*n² + 46*n - 48) / 12
     if n is odd:   a(n) = (8*n³ + 36*n² + 94*n - 138) / 12
   Initial values: a(0) = 1, a(1) = 9
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    seq_t N = (seq_t)n;
    seq_t num = (seq_t)8 * N * N * N;
    if ((n & 1ULL) == 0) {              /* even */
        num += (seq_t)12 * N * N;
        num += (seq_t)46 * N;
        num -= (seq_t)48;
    } else {                            /* odd */
        num += (seq_t)36 * N * N;
        num += (seq_t)94 * N;
        num -= (seq_t)138;
    }
    return num / (seq_t)12;
}
