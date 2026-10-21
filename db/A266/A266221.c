/*
 * A266221  Total number of ON (black) cells after n iterations of the "Rule
 *          7" elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n==0 then 1 else if n even: n*(n+1)/2 if n odd:
 *            (n+1)*(n+2)/2 (OEIS A266221)
 * Range:     u8 n <= 22, u16 n <= 360, u32 n <= 499, u64 n <= 499,
 *            u128 n <= 499, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if ((n & 1) == 0) {
        /* n is even */
        seq_t nn = (seq_t)n;
        return nn * ((seq_t)(n + 1)) / 2;
    } else {
        /* n is odd */
        return ((seq_t)(n + 1)) * ((seq_t)(n + 2)) / 2;
    }
}
