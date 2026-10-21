/*
 * A267046  Number of ON (black) cells in the n-th iteration of the "Rule 91"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=2; For n>2: if n is even a(n)=5, if n is
 *            odd a(n)=2*n-2
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)2;
    if ((n & 1) == 0)          /* even n > 2 */
        return (seq_t)5;
    else                       /* odd n > 2 */
        return (seq_t)2 * (seq_t)n - (seq_t)2;
}
