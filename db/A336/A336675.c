/*
 * A336675  Number of paths of length n starting at initial node of the path
 *          graph P_10.
 * https://oeis.org/A336675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) - 3*a(n-4) + a(n-5) a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=6
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial five terms */
    static const seq_t init[5] = { (seq_t)1, (seq_t)1, (seq_t)2,
                                  (seq_t)3, (seq_t)6 };

    if (n < 5u)
        return init[n];

    /* Working buffer for the last five computed values */
    seq_t buf[5];
    buf[0] = init[0];
    buf[1] = init[1];
    buf[2] = init[2];
    buf[3] = init[3];
    buf[4] = init[4];

    for (idx_t i = 5u; i <= n; ++i)
    {
        idx_t im1 = (i - 1u) % 5u;   /* a(n-1) */
        idx_t im2 = (i - 2u) % 5u;   /* a(n-2) */
        idx_t im3 = (i - 3u) % 5u;   /* a(n-3) */
        idx_t im4 = (i - 4u) % 5u;   /* a(n-4) */
        idx_t im5 = (i - 5u) % 5u;   /* a(n-5) */
        idx_t iod = i % 5u;          /* position for a(n) */

        seq_t sum = buf[im1] + (buf[im2] * (seq_t)4u) + buf[im5];
        seq_t sub = (buf[im3] * (seq_t)3u) + (buf[im4] * (seq_t)3u);
        buf[iod] = sum - sub;
    }

    return buf[n % 5u];
}
