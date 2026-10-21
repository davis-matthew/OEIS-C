/*
 * A272705  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 515", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = 1 + 12 n^2 a(2n-1) = 9 - 16 n + 12 n^2 where n is a
 *            non‑negative integer
 * Range:     u8 n <= 9, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle even indices: n = 2k */
    if ((n & 1) == 0) {
        seq_t k = (seq_t)(n >> 1);          /* k = n/2              */
        return (seq_t)1 + (seq_t)12 * k * k;
    }

    /* Handle odd indices: n = 2k-1  =>  k = (n+1)/2 */
    seq_t k = (seq_t)((n + 1) >> 1);
    return (seq_t)9 - (seq_t)16 * k + (seq_t)12 * k * k;
}
