/*
 * A269912  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 3", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = (2*n^3 + 3*n^2 - 5*n + 3) / 3 for n even * a(n) =
 *            (2*n^3 + 9*n^2 + 7*n) / 3 for n odd
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to the integer type used for sequence values */
    seq_t n_t = (seq_t)n;
    seq_t n2  = n_t * n_t;
    seq_t n3  = n2 * n_t;
    seq_t result;

    if ((n & 1ULL) == 0) {          /* even n */
        result = (2 * n3 + 3 * n2 - 5 * n_t + 3) / 3;
    } else {                        /* odd n */
        result = (2 * n3 + 9 * n2 + 7 * n_t) / 3;
    }

    return result;
}
