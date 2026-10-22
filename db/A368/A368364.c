/*
 * A368364  a(n) = number of s with n^k-n^2 <= s <= n^k-1, k >= 3, such that a
 *          comma sequence in base n with initial term s will not reach n^k.
 * https://oeis.org/A368364
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (m,j) : m≥1, j≥0, (m+1)(m+2)/2 – 1 + j·m ≤ n } minus 1
 *            (for n ≥ 2). This is equivalent to the generating function given
 *            in the OEIS entry
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined in OEIS; return 0 safely. */
    if (n < 2) {
        return 0;
    }
    seq_t count = 0;

    /* Count all pairs (m,j) satisfying the inequality. */
    for (idx_t m = 1; ; ++m) {
        /* Compute base = (m+1)(m+2)/2 – 1 without overflowing idx_t. */
        seq_t base_seq = ((seq_t)(m + 1) * (m + 2)) / (seq_t)2 - (seq_t)1;

        /* base_seq is guaranteed to fit in idx_t for the relevant m. */
        idx_t base = (idx_t)base_seq;

        if (base > n) {
            break;
        }

        idx_t diff = n - base;          /* all numbers ≤ n that can be reached for this m */
        idx_t jmax = diff / m;          /* maximum j satisfying for this m */
        count += (seq_t)(jmax + 1);     /* count j = 0 … jmax */
    }

    /* Subtract the single offset introduced in the generating function. */
    return count - 1;
}
