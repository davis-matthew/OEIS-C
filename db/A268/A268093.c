/*
 * A268093  Number of 1 X n 0..2 arrays with every repeated value in every row
 *          not one larger and in every column one larger mod 3 than the
 *          previous repeated value, and upper left element zero.
 * https://oeis.org/A268093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3); a(1)=1, a(2)=3, a(3)=9
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 3) return 9;

    seq_t a1 = 1;   /* a_{i-3}  */
    seq_t a2 = 3;   /* a_{i-2}  */
    seq_t a3 = 9;   /* a_{i-1}  */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 4 * a3 - 2 * a2 - 4 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
