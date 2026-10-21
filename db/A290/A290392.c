/*
 * A290392  Number of 6-cycles in the n-triangular honeycomb obtuse knight
 *          graph.
 * https://oeis.org/A290392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n=1..4; a(5)=4; a(6)=13; a(7)=98; a(8)=415; for
 *            n>=9: a(n) = 16001 - 4323*n + 297*n^2
 * Range:     u8 n <= 7, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 8) {
        static const seq_t vals[8] = {0, 0, 0, 0, 4, 13, 98, 415};
        return vals[n - 1];
    }
    seq_t nn = (seq_t)n;
    /* Compute 16001 + 297*n^2 - 4323*n without overflow for 64‑bit types. */
    seq_t val = 16001 + 297 * nn * nn - 4323 * nn;
    return val;
}
