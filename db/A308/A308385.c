/*
 * A308385  a(n) is the last square visited by fers moves on a spirally
 *          numbered (2n-1) X (2n-1) board, moving to the lowest available
 *          unvisited square at each step.
 * https://oeis.org/A308385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n^2-9n+6)*[n odd] + (4n^2-11n+9)*[n even]
 * Range:     u8 n <= 9, u16 n <= 129, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t value = 4 * nn * nn;
    if (n & 1) {          /* n is odd */
        value -= 9 * nn;
        value += 6;
    } else {              /* n is even */
        value -= 11 * nn;
        value += 9;
    }
    return value;
}
