/*
 * A337519  Length of the shortest walk in an n X n grid graph that starts in
 *          one corner and visits every edge.
 * https://oeis.org/A337519
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 - 4 if n even; a(n) = 2*n^2 - 3 if n odd
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(2) = 4, a(3) = 15, a(4) = 28, ... (offset 2) */
seq_t a(idx_t n)
{
    /* Ensure n >= 2; behaviour for n < 2 is undefined. */
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)2 * nn * nn;
    if ((n & 1) == 0) {          /* even n */
        val -= (seq_t)4;
    } else {                     /* odd n */
        val -= (seq_t)3;
    }
    return val;
}
