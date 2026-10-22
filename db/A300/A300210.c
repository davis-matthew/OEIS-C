/*
 * A300210  Number of n X 3 0..1 arrays with every element equal to 0, 1, 2,
 *          3, 4, 5, 7 or 8 king-move adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A300210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (by empirical recurrence):
   a(n) = 8*a(n-1) - 4*a(n-2) - 8*a(n-3) - 15*a(n-4)
          - 4*a(n-5) - 25*a(n-6) - 44*a(n-7) - 12*a(n-8),
   with initial values:
   a(1)=4, a(2)=32, a(3)=228, a(4)=1651, a(5)=11965, a(6)=86775,
   a(7)=629440, a(8)=4566023.
   The function recomputes a(1)…a(n) in O(n) time. */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* offset is 1, values below are undefined */

    /* initial values for n = 1 … 8 */
    static const seq_t init[8] = {
        (seq_t)4,
        (seq_t)32,
        (seq_t)228,
        (seq_t)1651,
        (seq_t)11965,
        (seq_t)86775,
        (seq_t)629440,
        (seq_t)4566023
    };

    if (n <= 8)
        return init[n-1];

    /* store the last eight computed values */
    seq_t last[8];
    for (idx_t i = 0; i < 8; ++i)
        last[i] = init[i];

    /* iterate from 9 up to n */
    for (idx_t i = 9; i <= n; ++i) {
        seq_t val =
            (seq_t)8  * last[7]   -
            (seq_t)4  * last[6]   -
            (seq_t)8  * last[5]   -
            (seq_t)15 * last[4]   -
            (seq_t)4  * last[3]   -
            (seq_t)25 * last[2]   -
            (seq_t)44 * last[1]   -
            (seq_t)12 * last[0];
        /* shift the window */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = last[6];
        last[6] = last[7];
        last[7] = val;
    }

    return last[7];   /* value a(n) */
}
