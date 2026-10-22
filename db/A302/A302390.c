/*
 * A302390  Triameter of the n-cube-connected cycle graph.
 * https://oeis.org/A302390
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*(n/2) - 4 for even n >= 4 a(n) = 12*((n-1)/2) for odd
 *            n >= 7 a(3) = 13, a(5) = 25 (indices below 3 are undefined in
 *            OEIS, return 0)
 * Range:     u8 n <= 43, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 3) return (seq_t)13;
    if (n == 5) return (seq_t)25;
    if (n < 3)  return (seq_t)0;  /* undefined case */

    if ((n & 1) == 0) {            /* even index */
        seq_t k = (seq_t)(n >> 1);    /* k = n/2 */
        return 12 * k - 4;
    } else {                     /* odd index >=7 */
        seq_t k = (seq_t)((n - 1) >> 1); /* k = (n-1)/2 */
        return 12 * k;
    }
}
