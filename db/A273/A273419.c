/*
 * A273419  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 705", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (111-3*(-1)^n-58*n+48*n^2+16*n^3)/12 for n>1; a(0) = 1,
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
    /* cast the index to seq_t so all arithmetic stays in seq_t */
    seq_t sn = (seq_t)n;          /* n         */
    seq_t n2 = sn * sn;           /* n^2       */
    seq_t n3 = n2 * sn;           /* n^3       */
    /* compute 111 - 3*(-1)^n */
    seq_t base = (n & 1) ? (seq_t)114 : (seq_t)108; /* 111 +/- 3 */
    /* numerator = base - 58*n + 48*n^2 + 16*n^3 */
    seq_t numerator = base
                      - (seq_t)58 * sn
                      + (seq_t)48 * n2
                      + (seq_t)16 * n3;
    return numerator / (seq_t)12;
}
