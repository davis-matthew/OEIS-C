/*
 * A292344  The first Zagreb index of the Aztec diamond AZ(n) (see the Ramanes
 *          et al. reference, Theorem 2.1).
 * https://oeis.org/A292344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*n^2 + 48*n - 12
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t r = 32 * nn;   /* 32*n */
    r = r * nn;          /* 32*n^2 */
    r = r + 48 * nn;     /* +48*n */
    r = r - 12;          /* -12 */
    return r;
}
