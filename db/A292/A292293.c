/*
 * A292293  Number of vertices of type D at level n of the hyperbolic Pascal
 *          pyramid.
 * https://oeis.org/A292293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 37*a(n-2) + 37*a(n-3) - 12*a(n-4) + a(n-5)
 *            Initial values: a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=3,a(5)=24
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n < 6 */
    static const seq_t init[6] = {0, 0, 0, 0, 3, 24};
    if (n < 6)
        return init[n];

    /* Buffer holding the last 5 computed values:
       arr[0] = a(n-5), arr[1] = a(n-4), arr[2] = a(n-3),
       arr[3] = a(n-2), arr[4] = a(n-1)      */
    seq_t arr[5];
    arr[0] = init[1];   /* a(1) */
    arr[1] = init[2];   /* a(2) */
    arr[2] = init[3];   /* a(3) */
    arr[3] = init[4];   /* a(4) */
    arr[4] = init[5];   /* a(5) */

    seq_t val = 0;
    for (idx_t i = 6; i <= n; ++i) {
        val = 12 * arr[4] - 37 * arr[3] + 37 * arr[2] - 12 * arr[1] + arr[0];
        /* Shift the buffer left for the next iteration */
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = arr[3];
        arr[3] = arr[4];
        arr[4] = val;
    }

    return val;
}
