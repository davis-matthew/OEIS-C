/*
 * A303619  Number of nX3 0..1 arrays with every element equal to 0, 2, 3, 4,
 *          5 or 6 horizontally, diagonally or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A303619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) + 8*a(n-3) - 12*a(n-4) + 4*a(n-5) -
 *            4*a(n-6) (n>8) Initial values: a(1)=1, a(2)=12, a(3)=38,
 *            a(4)=148, a(5)=616, a(6)=2520, a(7)=10288, a(8)=42100
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* first eight terms */
    static const seq_t init[8] = {1, 12, 38, 148, 616, 2520, 10288, 42100};

    if (n <= 8) {
        return init[n - 1];
    }

    /* variables holding the six previous terms (a(n-6) to a(n-1)) */
    seq_t a3 = init[2];  /* a(n-6)   */
    seq_t a4 = init[3];  /* a(n-5)   */
    seq_t a5 = init[4];  /* a(n-4)   */
    seq_t a6 = init[5];  /* a(n-3)   */
    seq_t a7 = init[6];  /* a(n-2)   */
    seq_t a8 = init[7];  /* a(n-1)   */

    for (idx_t i = 9; i <= n; ++i) {
        seq_t next = 5 * a8 - 5 * a7 + 8 * a6 - 12 * a5 + 4 * a4 - 4 * a3;
        /* shift the window forward */
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = a8;
        a8 = next;
    }
    return a8;
}
