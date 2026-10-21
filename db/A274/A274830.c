/*
 * A274830  Numbers k such that 7*k+1 is a triangular number (A000217).
 * https://oeis.org/A274830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(7*n - 6)/8 if n even, (n - 1)*(7*n - 1)/8 if n odd (n
 *            ≥ 1)
 * Range:     u8 n <= 17, u16 n <= 274, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not used, but safe */
    seq_t n_t = (seq_t)n;
    if ((n_t & 1) == 0) {          /* even n */
        return (n_t * ((seq_t)7 * n_t - (seq_t)6)) / (seq_t)8;
    } else {                       /* odd n */
        return ((n_t - (seq_t)1) * ((seq_t)7 * n_t - (seq_t)1)) / (seq_t)8;
    }
}
