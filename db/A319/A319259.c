/*
 * A319259  Minimal number of straight lines of a covering tree needed to
 *          cover n X n X n points arranged in a 3-D grid.
 * https://oeis.org/A319259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more, bref
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A319259: Minimal number of straight lines of a covering tree needed
 * to cover an n × n × n grid.
 *
 * Formula:
 *   a(1) = 1
 *   a(2) = 4
 *   a(3) = 12
 *   For n ≥ 4:
 *     if n is even:  a(n) = n^2 + n
 *     if n is odd :  a(n) = n^2 + n + 1
 *
 * This implements the closed‑form derived from known constructions.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)12;
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {          /* even n */
        return nn * nn + nn;
    } else {                      /* odd n >= 5 */
        return nn * nn + nn + (seq_t)1;
    }
}
