/*
 * A269910  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 3", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A269910: a(0)=1, a(n)=1 if n even; a(n)=4*n^2+4*n-3 if n odd */

seq_t a(idx_t n)
{
    if ((n & 1) == 0)         /* n is even */
        return 1;

    seq_t nn = (seq_t)n;      /* cast index to sequence type */
    /* compute 4*n*n + 4*n - 3 using seq_t arithmetic */
    return 4 * nn * nn + 4 * nn - 3;
}
