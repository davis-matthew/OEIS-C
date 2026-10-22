/*
 * A307208  a(n) is the forgotten index of the Fibonacci cube Gamma(n).
 * https://oeis.org/A307208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 8*a(n-3) + 5*a(n-4) + 8*a(n-5) -
 *            2*a(n-6) - 4*a(n-7) - a(n-8) a(1)=2, a(2)=10, a(3)=52, a(4)=158,
 *            a(5)=466, a(6)=1192, a(7)=2914, a(8)=6722
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base terms for indices 1..8 */
    static const seq_t base[8] = {2, 10, 52, 158, 466, 1192, 2914, 6722};

    if (n <= 8)
        return base[n - 1];

    /* Circular buffer holding the last 8 computed values:
     * t[0] = a(i-8), t[1] = a(i-7), ..., t[7] = a(i-1)     */
    seq_t t[8];
    for (idx_t i = 0; i < 8; ++i)
        t[i] = base[i];

    for (idx_t i = 9; i <= n; ++i) {
        seq_t new_val =
            4 * t[7]          /*  a(i-1) */
          - 2 * t[6]          /*  a(i-2) */
          - 8 * t[5]          /*  a(i-3) */
          + 5 * t[4]          /*  a(i-4) */
          + 8 * t[3]          /*  a(i-5) */
          - 2 * t[2]          /*  a(i-6) */
          - 4 * t[1]          /*  a(i-7) */
          -   t[0];           /*  a(i-8) */

        /* Shift buffer left: discard a(i-8), move others left */
        t[0] = t[1];
        t[1] = t[2];
        t[2] = t[3];
        t[3] = t[4];
        t[4] = t[5];
        t[5] = t[6];
        t[6] = t[7];
        t[7] = new_val;
    }

    return t[7];
}
