/*
 * A328148  The number of conics tangent to five non-singular curves of degree
 *          n in general position, in a projective plane defined over an
 *          algebraically closed field of characteristic zero.
 * https://oeis.org/A328148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 * ((n-1)^5 + 10*(n-1)^4 + 40*(n-1)^3 + 40*(n-1)^2 +
 *            10*(n-1) + 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 83, u128 n <= 7131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* For n=0 (outside OEIS offset) return 0; not expected to be called. */
    if (n == 0) return (seq_t)0;
    seq_t m = (seq_t)n - (seq_t)1;          /* m = n-1                                                */
    seq_t m2 = m * m;                       /* m^2 */
    seq_t m3 = m2 * m;                      /* m^3 */
    seq_t m4 = m3 * m;                      /* m^4 */
    seq_t m5 = m4 * m;                      /* m^5 */
    /* Polynomial in (n-1) */
    seq_t poly = m5
               + (seq_t)10 * m4
               + (seq_t)40 * m3
               + (seq_t)40 * m2
               + (seq_t)10 * m
               + (seq_t)1;
    /* n^5 */
    seq_t n2 = (seq_t)n * (seq_t)n;         /* n^2 */
    seq_t n3 = n2 * (seq_t)n;               /* n^3 */
    seq_t n4 = n3 * (seq_t)n;               /* n^4 */
    seq_t n5 = n4 * (seq_t)n;               /* n^5 */
    return n5 * poly;
}
