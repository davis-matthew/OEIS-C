/*
 * A249983  Number of length 3+1 0..2*n arrays with the sum of the absolute
 *          values of adjacent differences equal to 3*n.
 * https://oeis.org/A249983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+a(n-2)-4*a(n-3)+a(n-4)+2*a(n-5)-a(n-6); Initial
 *            values: a(1)=20, a(2)=88, a(3)=208, a(4)=426, a(5)=728,
 *            a(6)=1178
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[6] = {20, 88, 208, 426, 728, 1178};

    if (n == 0)          /* offset is 1; value for n<1 undefined, return 0 */
        return 0;
    if (n <= 6)
        return init[n - 1];

    seq_t r[6];
    for (int k = 0; k < 6; ++k)
        r[k] = init[k];

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = 2 * r[5] + r[4] - 4 * r[3] + r[2] + 2 * r[1] - r[0];
        if (i == n)
            return next;
        /* shift window: a(i-5)...a(i-1) becomes a(i-4)...a(i) */
        r[0] = r[1];
        r[1] = r[2];
        r[2] = r[3];
        r[3] = r[4];
        r[4] = r[5];
        r[5] = next;
    }

    return 0;  /* unreachable, but keeps compiler happy */
}
