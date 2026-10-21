/*
 * A270166  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 107", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3+9*n^2+5*n-108)/6 for even n>6 * a(n) =
 *            (4*n^3+18*n^2+23*n-99)/6 for odd n>6 * First terms a(0)..a(6)
 *            are stored in the table
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[] = {
        (seq_t)1,   /* a(0) */
        (seq_t)6,   /* a(1) */
        (seq_t)11,  /* a(2) */
        (seq_t)52,  /* a(3) */
        (seq_t)64,  /* a(4) */
        (seq_t)173, /* a(5) */
        (seq_t)189  /* a(6) */
    };
    if (n <= 6)
        return init[n];

    seq_t nseq = (seq_t)n;
    seq_t n_sq = nseq * nseq;          /* n^2 */
    seq_t n_cu = n_sq * nseq;          /* n^3 */
    seq_t num;
    if ((n & 1) == 0) {                /* even n */
        num = 4 * n_cu + 9 * n_sq + 5 * nseq - (seq_t)108;
    } else {                           /* odd n */
        num = 4 * n_cu + 18 * n_sq + 23 * nseq - (seq_t)99;
    }
    return num / (seq_t)6;
}
