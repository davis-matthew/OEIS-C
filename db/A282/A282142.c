/*
 * A282142  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 553", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(n)=0 for odd n; a(n)=concatenation of n/2 copies of
 *            "10" followed by "0" for even n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n & 1ULL) return (seq_t)0;
    idx_t half = n >> 1;
    seq_t val = 0;
    for (idx_t i = 0; i < half; ++i) {
        val = val * (seq_t)10 + (seq_t)1;
        val = val * (seq_t)10 + (seq_t)0;
    }
    val = val * (seq_t)10 + (seq_t)0;
    return val;
}
