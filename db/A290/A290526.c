/*
 * A290526  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 833", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 110*a(n-2) - 110*a(n-3) - 1000*a(n-4) +
 *            1000*a(n-5) (with additive corrections for n = 6 and 7): a(6) :
 *            + 110000 a(7) : – 100000 Initial values: a(0) = 1 a(1) = 1 a(2)
 *            = 101 a(3) = 1111 a(4) = 11 (The homogeneous recurrence holds
 *            for n ≥ 8.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t base[5] = {1, 1, 101, 1111, 11};
    if (n < 5) return base[n];

    /* prev[0] = a(k-1), prev[1] = a(k-2), ..., prev[4] = a(k-5) at iteration k */
    seq_t prev[5];
    prev[0] = base[4]; /* a4 */
    prev[1] = base[3]; /* a3 */
    prev[2] = base[2]; /* a2 */
    prev[3] = base[1]; /* a1 */
    prev[4] = base[0]; /* a0 */

    for (idx_t i = 5; i <= n; ++i) {
        /* Compute the affine combination */
        seq_t sum = prev[0] + 110 * prev[1] + 1000 * prev[4];
        if (i == 6) {
            sum += (seq_t)110000;
        } else if (i == 7) {
            sum -= (seq_t)100000;
        }
        seq_t sub = 110 * prev[2] + 1000 * prev[3];
        seq_t cur = sum - sub;

        /* Shift the circular buffer */
        prev[4] = prev[3];
        prev[3] = prev[2];
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = cur;
    }
    return prev[0];
}
