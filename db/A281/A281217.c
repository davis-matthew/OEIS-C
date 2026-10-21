/*
 * A281217  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 342", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A281217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4k) = (16^(k+1) - 1) / 15 a(4k+1) = ⌊13 * a(4k) / 4⌋ a(4k+2) =
 *            a(4k) a(4k+3) = 7 * a(4k) (where ⌊x⌋ is integer floor division)
 *            The formula yields the OEIS entry A281217
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t k = n / 4;          /* integer division */
    int   r = n & 3;          /* remainder modulo 4 */
    /* Compute b = a(4k) = sum_{i=0..k} 16^i */
    seq_t pow16 = (seq_t)1;
    seq_t b = (seq_t)0;
    for (idx_t i = 0; i <= k; ++i) {
        b += pow16;
        pow16 *= (seq_t)16;
    }
    /* Return value according to remainder */
    switch (r) {
        case 0:
        case 2:
            return b;
        case 1:
            return ((seq_t)13 * b) / (seq_t)4;
        case 3:
            return (seq_t)7 * b;
    }
    return (seq_t)0;  /* unreachable */
}
