/*
 * A293933  Poincaré series for invariant polynomial functions on the space of
 *          binary forms of degree 7.
 * https://oeis.org/A293933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (11*n^4 + 88*n^3 + 768*n^2 + 3168*n + 8640)/8640 *
 *            - ((n mod 2)*(n+1)*(n+3)*11)/96 * - ((n mod 3)*n*5)/54 )
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 631, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* cast n to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    /* compute powers of n */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;

    /* main numerator */
    seq_t main = (seq_t)11 * n4
               + (seq_t)88 * n3
               + (seq_t)768 * n2
               + (seq_t)3168 * nn
               + (seq_t)8640;

    /* B = (n mod 2)*(n+1)*(n+3)*11 */
    seq_t B = ((seq_t)(nn & 1)) * (nn + (seq_t)1) * (nn + (seq_t)3) * (seq_t)11;

    /* C = (n mod 3)*n*5 */
    seq_t C = ((seq_t)(nn % (seq_t)3)) * nn * (seq_t)5;

    /* compute full numerator: main - B*90 - C*160 */
    seq_t result_numer = main
                       - B * (seq_t)90
                       - C * (seq_t)160;

    /* return floor of result_numer / 8640 */
    return result_numer / (seq_t)8640;
}
