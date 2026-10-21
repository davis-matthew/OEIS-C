/*
 * A290193  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 705", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 101; for n ≥ 3, if n is even: a(n) =
 *            (10^(n–1) – 1)/9 (decimal number consisting of n–1 ones); if n
 *            is odd: a(n) = 10·10^(n–1) + (10^(n–1) – 1)/9 (decimal number
 *            that starts with “10” followed by n–1 ones)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1)
        return (seq_t)1;

    if (n == 2) {
        seq_t val = 0;
        val = val * (seq_t)10 + (seq_t)1;
        val = val * (seq_t)10 + (seq_t)0;
        val = val * (seq_t)10 + (seq_t)1;
        return val;
    }

    seq_t val = 0;
    if (n & 1) { /* odd index */
        val = val * (seq_t)10 + (seq_t)1;   /* leading '1' */
        val = val * (seq_t)10 + (seq_t)0;   /* followed by '0' */
        for (idx_t i = 0; i < n - 1; ++i)
            val = val * (seq_t)10 + (seq_t)1;   /* n-1 ones after the leading 10 */
    } else { /* even index (n ≥ 4) */
        for (idx_t i = 0; i < n - 1; ++i)
            val = val * (seq_t)10 + (seq_t)1;   /* n-1 ones */
    }

    return val;
}
