/*
 * A301709  Partial sums of A301708.
 * https://oeis.org/A301709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1)-4*a(n-2)+4*a(n-3)-3*a(n-4)+a(n-5) a(0) = 1, a(1)
 *            = 7, a(2) = 18, a(3) = 34, a(4) = 56
 * Range:     u8 n <= 9, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 7;
    if (n == 2) return 18;
    if (n == 3) return 34;
    if (n == 4) return 56;

    seq_t buf[5] = {56, 34, 18, 7, 1}; /* buf[0]=a(n-1), buf[1]=a(n-2), … buf[4]=a(n-5) */
    seq_t val = 0;

    for (idx_t i = 5; i <= n; ++i) {
        val = 3 * buf[0] - 4 * buf[1] + 4 * buf[2] - 3 * buf[3] + buf[4];
        for (int k = 4; k >= 1; --k)
            buf[k] = buf[k - 1];
        buf[0] = val;
    }
    return val;
}
