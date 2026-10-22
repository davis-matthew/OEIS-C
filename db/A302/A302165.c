/*
 * A302165  Number of 3 X n 0..1 arrays with every element equal to 0, 1 or 3
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + 2*a(n-4) for n > 6 initial
 *            values: a(1)=4, a(2)=6, a(3)=7, a(4)=12, a(5)=20, a(6)=34
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 85, u128 n <= 172,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[6] = {4, 6, 7, 12, 20, 34};

    if (n <= 6)
        return init[n - 1];

    /* a(3) .. a(6) hold a(n-3) .. a(n) respectively */
    seq_t a2 = init[2];   /* a(3)  */
    seq_t a3 = init[3];   /* a(4)  */
    seq_t a4 = init[4];   /* a(5)  */
    seq_t a5 = init[5];   /* a(6)  */

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute a(i) = a(i-1) + a(i-2) - a(i-3) + 2*a(i-4) */
        seq_t next = a5 + a4 - a3 + (seq_t)2 * a2;

        /* Shift values for next iteration */
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5;
}
