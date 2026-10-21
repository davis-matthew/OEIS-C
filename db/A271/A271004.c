/*
 * A271004  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 245", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A271004 a(n) = { 1, 8, 4, 44, 13, 112 } for n <= 5 a(n) = 6*n -
 *            11 if n is even and n > 3 a(n) = 4*n^2 + 2*n - 2 if n is odd and
 *            n > 5 These closed forms were conjectured by Colin Barker and
 *            verified for all n up to the range of the sequence's b-file
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values for n = 0 .. 5 */
    static const seq_t init[6] = { 1, 8, 4, 44, 13, 112 };

    if (n <= 5)
        return init[n];

    /* cast index once for arithmetic */
    seq_t nn = (seq_t)n;

    if ((n & 1) == 0)        /* even n > 3 */
        return 6*nn - 11;

    /* odd n > 5 */
    return 4*nn*nn + 2*nn - 2;
}
