/*
 * A252178  Number of length 2+2 0..n arrays with the sum of the maximum minus
 *          the minimum of adjacent triples multiplied by some arrangement of
 *          +-1 equal to zero.
 * https://oeis.org/A252178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 14*n^3 + 29*n^2 + 22*n + 6)/6, Offset: 1
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculates a(n) for n >= 1, as defined by OEIS A252178.  Function signature
   follows the build's expected types: seq_t is the output type and idx_t is
   an unsigned integer type such as uint64_t.  The implementation uses only
   integer arithmetic (no floating point) and therefore produces exact
   values for all n that fit in the chosen seq_t type. */
seq_t a(idx_t n) {
    /* n = 0 is outside the defined offset; returning 0 is harmless. */
    if (n == 0) return (seq_t)0;

    /* Work entirely in seq_t to keep arithmetic exact across all build
       configurations (uint8_t, uint16_t, uint32_t, uint64_t, __int128,
       arbitrary precision). */
    seq_t N   = (seq_t)n;
    seq_t Ns  = N * N;        /* n^2 */
    seq_t Nss = Ns * N;       /* n^3 */
    seq_t Nsss= Nss * N;      /* n^4 */

    /* Numerator: n^4 + 14*n^3 + 29*n^2 + 22*n + 6 */
    seq_t num = Nsss
              + (seq_t)14 * Nss
              + (seq_t)29 * Ns
              + (seq_t)22 * N
              + (seq_t)6;

    /* Final value is numerator divided by 6.  The division is exact. */
    return num / (seq_t)6;
}
