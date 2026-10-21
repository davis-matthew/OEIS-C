/*
 * A298040  Coordination sequence of Dual(4.6.12) tiling with respect to a
 *          tetravalent node.
 * https://oeis.org/A298040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4; for n>1: a(n)=10*n if n even, 8*n if n odd
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the special first two terms */
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)4;
    }

    /* For n >= 2 use the closed‑form formulas */
    if ((n & 1) == 0) {        /* n even */
        return (seq_t)10 * (seq_t)n;
    } else {                   /* n odd */
        return (seq_t)8 * (seq_t)n;
    }
}
