/*
 * A285612  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 62", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1, a(1) = 1;
 * for n >= 2, let k = floor(n/2).  Then
 *   a(n) = 10 * (10^k - 1) / 9   (mimics the binary string of k '1's followed by a '0')
 *
 * This is equivalent to the OEIS formulas:
 *   a(2k) = 10 * (10^k - 1) / 9   for k >= 1
 *   a(2k+1) = (10^(k+1) - 10) / 9 for k >= 1
 *
 * The implementation builds the decimal number directly by repeated
 * multiplication by 10 and addition of 1 to generate the string of
 * k ones, then appends a trailing zero.  This works for any seq_t
 * width (including arbitrary‑precision) and is efficient for the
 * required range.
 */

seq_t a(idx_t n) {
    if (n <= 1) {
        return (seq_t)1;
    }

    /* number of leading '1's */
    idx_t k = n / 2;

    seq_t val = (seq_t)0;
    for (idx_t i = 0; i < k; ++i) {
        val = val * (seq_t)10 + (seq_t)1;
    }
    val = val * (seq_t)10;  /* append trailing '0' */

    return val;
}
