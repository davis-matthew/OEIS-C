/*
 * A253547  Total number of star-shaped dodecagons appearing in a variant of
 *          hexagon expansion after n iterations.
 * https://oeis.org/A253547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (27 - 3*(-1)^n - 28*n + 6*n^2)/8 for n>5, with initial
 *            values a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=3
 * Range:     u8 n <= 8, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the initial terms explicitly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)0;
    if (n == 4) return (seq_t)1;
    if (n == 5) return (seq_t)3;

    /* For n > 5 use the closed‑form formula */
    seq_t nn = (seq_t)n;
    seq_t term = (seq_t)27;

    /* Add or subtract 3 depending on parity of n */
    if (n & 1U) {
        term += (seq_t)3;   /* odd n: (-1)^n = -1 -> +3 */
    } else {
        term -= (seq_t)3;   /* even n: (-1)^n =  1 -> -3 */
    }

    term -= (seq_t)28 * nn;
    term += (seq_t)6 * nn * nn;

    return term / (seq_t)8;
}
