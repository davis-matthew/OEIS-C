/*
 * A276391  G.f. satisfies A(x) - 4*A(x^2) = x/(1+x).
 * https://oeis.org/A276391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 if n is odd; otherwise a(2n) = 4*a(n) – 1. Base case:
 *            a(1) = 1. No other precomputation is needed
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 9447, u64 n <= 9447,
 *            u128 n <= 9447, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)                 /* Offset is 1; 0 is not defined. */
        return (seq_t)0;
    if (n & 1)                  /* odd index → value 1 */
        return (seq_t)1;
    /* even index → use recursive formula a(n) = 4*a(n/2) – 1 */
    return 4 * a(n >> 1) - (seq_t)1;
}
