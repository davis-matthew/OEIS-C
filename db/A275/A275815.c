/*
 * A275815  Maximum total number of possible moves that any number of queens
 *          of the same color can make on an n X n chessboard.
 * https://oeis.org/A275815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 8·(n-2)² for n ≥ 6; a(1)=0, a(2)=4, a(3)=17, a(4)=40,
 *            a(5)=76
 * Range:     u8 n <= 7, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first few values explicitly */
    if (n == 1) return 0;
    if (n == 2) return 4;
    if (n == 3) return 17;
    if (n == 4) return 40;
    if (n == 5) return 76;

    /* For n ≥ 6 use the conjectured formula */
    seq_t t = (seq_t)(n - 2);
    return (seq_t)8 * t * t;
}
