/*
 * A292051  Wiener index of the n X n black bishop graph.
 * https://oeis.org/A292051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 - 10*n + 6*n^2 - 8*n^3 + 6*n^4 - 3*(-1)^n*(1 - 2*n +
 *            2*n^2))/24
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = n;
    seq_t N2 = N * N;        /* n^2 */
    seq_t N3 = N2 * N;       /* n^3 */
    seq_t N4 = N3 * N;       /* n^4 */

    seq_t numerator;
    if (n & 1) { /* odd n */
        seq_t termA = 6 * N4 + 12 * N2 + (seq_t)6;      /* 6n^4 +12n^2 +6 */
        seq_t termB = 8 * N3 + 16 * N;                 /* 8n^3 +16n */
        numerator = termA - termB;
    } else {     /* even n */
        seq_t termA = 6 * N4;                          /* 6n^4 */
        seq_t termB = 8 * N3 + 4 * N;                 /* 8n^3 +4n */
        numerator = termA - termB;
    }

    return numerator / 24;
}
