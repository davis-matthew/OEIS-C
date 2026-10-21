/*
 * A274681  Numbers k such that 4*k + 1 is a triangular number.
 * https://oeis.org/A274681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^2 - 3*n)/2 if n even; a(n) = (4*n^2 - 5*n + 1)/2 if
 *            n odd
 * Range:     u8 n <= 8, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n is 1-based index */
    seq_t nn = (seq_t)n;
    if ( (n & 1) == 0 ) {                 /* even n */
        seq_t term = (seq_t)4 * nn * nn   /* 4*n^2  */
                   - (seq_t)3 * nn;       /* -3*n   */
        return term / 2;                  /* divisibility guaranteed */
    } else {                              /* odd n */
        seq_t term = (seq_t)4 * nn * nn   /* 4*n^2  */
                   - (seq_t)5 * nn + 1;   /* -5*n+1 */
        return term / 2;                  /* divisibility guaranteed */
    }
}
